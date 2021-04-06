#include "XhGcodeFileParser.h"

#include <QFile>
#include <QByteArray>
#include <QChar>
#include <QMutexLocker>
#include <QDebug>
#include <QTextStream>

XhGcodeFileParser::XhGcodeFileParser(QObject *parent) : QThread(parent)
{
    m_headerInfo.insert("result", false);
    m_headerInfo.insert("mode", "Unknown");
    m_headerInfo.insert("offset", "0");
    m_headerInfo.insert("left_temp", "0");
    m_headerInfo.insert("right_temp", "0");
    m_headerInfo.insert("bed_temp", "0");
}

XhGcodeFileParser::~XhGcodeFileParser()
{
    delete m_sourceGcodeLines;
    delete m_mutex;
}

void XhGcodeFileParser::parseByDirect(const QString &inputFileName, const QString &outputFileName)
{
    m_mutex->lock();
    m_inputFileName = inputFileName;
    m_outputFileName = outputFileName;
    m_mutex->unlock();
    m_parseType.store((int)ParseType::Direct);

    start();
}

void XhGcodeFileParser::parseByDeep(const QString &inputFileName, const QString &outputFileName)
{
    m_mutex->lock();
    m_inputFileName = inputFileName;
    m_outputFileName = outputFileName;
    m_mutex->unlock();
    m_parseType.store((int)ParseType::Deep);

    start();
}

int XhGcodeFileParser::GetParsedLine()
{
    if(line_parsed >= (m_sourceGcodeLines->size() - 1))
    {
        return 100;
    }
    else
    {
        return (int)(line_parsed * 100 / m_sourceGcodeLines->size());
    }
}

void XhGcodeFileParser::run()
{
    ParseType parseType = (ParseType)m_parseType.load();

    switch (parseType) {
    case ParseType::Direct:
        doParseByDirect();
        break;
    case ParseType::Deep:
        doParseByDeep();
        break;
    default:
        break;
    }
    writeNewFile();

    m_mutex->lock();
    QString outputFileName = m_outputFileName;
    m_mutex->unlock();
    emit parseSucceded(outputFileName);


}

void XhGcodeFileParser::doParseByDirect()
{
    variableInit();
    loadFile();
    getJlHeader();

    if (m_headerInfo["result"] == false)
    {
        parseTop100Lines();
        int l  = m_headerInfo["left_temp"].toString().toInt();
        m_headerInfo["right_temp"] = QString::number(l+m_tempOffset);
#ifdef DEBUG
        qDebug()<<"left_temp"<<l<<m_headerInfo.value("left_temp").toString();
        qDebug()<<"mtempOffset"<<m_tempOffset;
        qDebug()<<"right_temp"<<m_headerInfo.value("right_temp").toString();
#endif
    }
    emit parseByDirectMode(m_headerInfo.value("mode").toString());
    emit parseByDeepHeader(m_headerInfo.value("left_temp").toString(),m_headerInfo.value("right_temp").toString(),m_headerInfo.value("bed_temp").toString(),m_headerInfo.value("offset").toString());
//    exit();
}

void XhGcodeFileParser::doParseByDeep()
{
    qDebug()<<"deep1";
    doParseByDirect();
    qDebug()<<"deep2";
    parseWholeFile();
    emit parseByDeepMode(m_headerInfo.value("mode").toString());
    emit parseByDeepHeader(m_headerInfo.value("left_temp").toString(),\
                           m_headerInfo.value("right_temp").toString(),\
                           m_headerInfo.value("bed_temp").toString(),\
                           m_headerInfo.value("offset").toString());/* 2021/3/2/ by cbw */

    emit parseDeepSucceded();
//    exit();
}

void XhGcodeFileParser::variableInit()
{
    m_lineParsed = 0;
    m_sourceGcodeLines->clear();
    m_lastToolHead = 0;
    m_tempOffset = 0;
    m_startLineIndex = 0;

    m_headerInfo["result"] = false;
    m_headerInfo["mode"] = "Unknown";
    m_headerInfo["offset"] = "0";
    m_headerInfo["left_temp"] = "0";
    m_headerInfo["right_temp"] = "0";
    m_headerInfo["bed_temp"] = "0";
}

void XhGcodeFileParser::loadFile()
{
    m_mutex->lock();
    QString inputFileName = m_inputFileName;
    m_mutex->unlock();

    QFile in(inputFileName);
    if (!in.open(QIODevice::ReadOnly))
        return;

    QTextStream textStream(&in);
    textStream.setCodec("UTF-8");
    while (true) {
        if (textStream.atEnd())
            break;
        m_sourceGcodeLines->append(textStream.readLine());
    }

    in.close();
}

void XhGcodeFileParser::getJlHeader()
{
    if(!m_sourceGcodeLines[0].startsWith(";JL-ParsedResult"))
        return;

    m_headerInfo["result"] = true;
    QString firstLine = m_sourceGcodeLines->at(0);
    QStringList options = firstLine.split(" ", QString::SkipEmptyParts);
    options.removeFirst();
    m_headerInfo["mode"] = options[0].split(":")[1];
    m_headerInfo["offset"] = options[1].split(":")[1];
    m_headerInfo["left_temp"] = options[2].split(":")[1];
    m_headerInfo["right_temp"] = options[3].split(":")[1];
    m_headerInfo["bed_temp"] = options[4].split(":")[1];
    m_startLineIndex = 1;

}

void XhGcodeFileParser::parseTop100Lines()
{
    int lineIndex = 0;

    while (lineIndex < 100) {
        QString tmpLine = m_sourceGcodeLines->at(m_lineParsed).trimmed();
        qDebug()<<tmpLine;
        int curLineNumber = m_lineParsed;
        m_lineParsed++;
        if (tmpLine.startsWith(";"))
            continue;
        QString line_cut_comment = cutComment(tmpLine);
        parseStartLine(line_cut_comment, curLineNumber);
        parseTemp(line_cut_comment);
        parseToolHead(line_cut_comment);
        parseMode(line_cut_comment);
        lineIndex++;
    }

}

QString XhGcodeFileParser::cutComment(QString gcode)
{
    int commnent_start_index = 0;
    QString new_string;
    gcode = gcode.trimmed();
    commnent_start_index = gcode.indexOf(";",0) ;
    if(commnent_start_index == -1)
        new_string = gcode;
    else
        new_string = gcode.left(commnent_start_index);
    return new_string;
}

void XhGcodeFileParser::parseStartLine(QString &gcode, int lineNumber)
{
    if (m_startLineIndex == 0) {
        if (gcode.contains("Z") && (gcode.contains("G0") || gcode.contains("G1"))) {
            float zHeight = getSymbolValue("Z", gcode).toFloat();
            if (zHeight <= 0.3f)
                m_startLineIndex = lineNumber;
        }
    }
}

QString XhGcodeFileParser::getSymbolValue(QString symbol, QString string)
{
    bool charFound = false;
    QString strvalue = "";
    if (string.contains(symbol) == false)
        return strvalue;
    foreach(QChar tmpC, string) {
        if (charFound == true) {
            if(((tmpC >= '0') && (tmpC <= '9')) || (tmpC == '.') || (tmpC == '-') || (tmpC == '+'))
                strvalue.append(tmpC);
            else
                break;
            // if ((tmpC != ".") && (tmpC != "-") && (tmpC != "+")) {
            //     if (!tmpC.isNumber())
            //         break;
            //     strvalue.append(tmpC);
            // } else {
            //     strvalue.append(tmpC);
            // }
        } else if (tmpC == symbol){
            charFound = true;
        }
    }

    return strvalue;
}

void XhGcodeFileParser::parseTemp(QString &gcode)
{
    if ((gcode.contains("M104") || gcode.contains("M109")) && gcode.contains("S")) {
        QString tempValue = getSymbolValue("S", gcode);
        if (gcode.contains("T")) {
            int tIndex = gcode.indexOf("T") + 1;
            int nozzleIndex = QString("%1").arg(gcode.at(tIndex)).toInt();
            if (nozzleIndex == 0) {
                m_headerInfo["left_temp"] = tempValue;
            } else if (nozzleIndex == 1) {
                m_headerInfo["right_temp"] = tempValue;
            }
        } else {
            if (m_lastToolHead == 0)
                m_headerInfo["left_temp"] = tempValue;
            else if (m_lastToolHead == 1)
                m_headerInfo["right_temp"] = tempValue;
        }
    } else if ((gcode.contains("M140") || gcode.contains("M190")) && gcode.contains("S")) {
        QString tempValue = getSymbolValue("S", gcode);
        m_headerInfo["bed_temp"] = tempValue;
    }
}

void XhGcodeFileParser::parseToolHead(QString &gcode)
{
    if (gcode.contains("T"))
        m_lastToolHead = getSymbolValue("T",gcode).toInt();
}

void XhGcodeFileParser::parseMode(QString &gcode)
{
    if (gcode.contains("M605") && gcode.contains("S")) {
        auto mode = getSymbolValue("S", gcode);
        if (mode == "2") {
            m_headerInfo["mode"] = "Orgin-Duplicate";
            m_tempOffset = getSymbolValue("R", gcode).toInt();
            m_headerInfo["offset"] = getSymbolValue("X", gcode).toFloat();
            m_origin_duplucate_found = true;
             emit parseByDirectMode("Orgin-Duplicate");/* 2021/3/2/ by cbw */
        } else if (mode == "3") {
            m_headerInfo["mode"] = "Orgin-Mirror";
             emit parseByDirectMode("Orgin-Mirror");/* 2021/3/2/ by cbw */
        } else if (mode == "1") {
            m_headerInfo["mode"] = "Mix";
             emit parseByDirectMode("Mix");/* 2021/3/2/ by cbw */
        }
    }
}

void XhGcodeFileParser::parseWholeFile()
{
    if(m_headerInfo.value("mode") != "Unknown") {
        return;
    }
    float maxX = -999.0;
    float minX = 999.0;
    float lastX = 0.0;

    line_parsed = 0;
    for (int var=m_startLineIndex; var<m_sourceGcodeLines->size(); var++)
    {
        QString tmpLine = m_sourceGcodeLines->at(var);
        line_parsed = var;
        if (tmpLine.startsWith(";"))
            continue;
        if (parseMotionMode(tmpLine) == true)
            continue;
        float x = parseXValue(tmpLine);

        if (x != -999) {
            if (m_relativeMotion == false) {
                lastX = x;
            } else {
                lastX += x;
            }
            if (lastX > maxX) {
                maxX = lastX;
            } else if (lastX < minX) {
                minX = lastX;
            }
        }
    }

    float xCenter, xSize;
    if ((minX != 999) && (maxX != -999)) {
        xCenter = (maxX + minX) / 2.0f;
        xSize = maxX - minX;
        qDebug("Min:%0.2f Max:%0.2f", minX, maxX);
        qDebug("Size:%0.2f Center:%0.2f", xSize, xCenter);
    }

    if (xSize < 140.0f) {
        m_headerInfo["mode"] = "Mirror";
        m_headerInfo["offset"] = QString::number(75.0f - xCenter, 'f', 2);
    } else if (xSize < 145.0f) {
        m_headerInfo["mode"] = "Duplicate";
        m_headerInfo["offset"] = QString::number(75.0f - xCenter, 'f', 2);
    } else {
        m_headerInfo["mode"] = "Unsupport";
    }
}

bool XhGcodeFileParser::parseMotionMode(QString &gcode)
{
    if (gcode.contains("G91")) {
        m_relativeMotion = true;
    } else if (gcode.contains("G90")) {
        m_relativeMotion = false;
    } else {
        return false;
    }
    return true;
}

float XhGcodeFileParser::parseXValue(QString &gcode)
{
    float x = -999.0;
    if (gcode.contains("X") && (gcode.contains("G0") || gcode.contains("G1"))) {
        x = getSymbolValue("X", gcode).toFloat();
    }

    return x;
}

void XhGcodeFileParser::writeNewFile()
{
    m_mutex->lock();
    QString outputFileName = m_outputFileName;
    m_mutex->unlock();

    QFile f(outputFileName);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QTextStream out(&f);
    out.setCodec("UTF-8");
    out << header2StringLine() << "\r\n";
    for (int var=m_startLineIndex; var<m_sourceGcodeLines->size(); var++)
        out << m_sourceGcodeLines->at(var) << "\r\n";
    f.close();
}

QString XhGcodeFileParser::header2StringLine()
{
    QString retval = ";JL-ParsedResult";
    retval = retval + " MoreOption:" + m_headerInfo["mode"].toString();
    retval = retval + " Offset:" + m_headerInfo["offset"].toString();
    retval = retval + " Left_temp:" + m_headerInfo["left_temp"].toString();
    retval = retval + " Right_temp:" + m_headerInfo["right_temp"].toString();
    retval = retval + " Bed_temp:" + m_headerInfo["bed_temp"].toString();

    return retval;
}
