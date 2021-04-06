import QtQuick 2.9
import QtQuick.Window 2.2
//import QtQuick.Controls 2.2

Rectangle {
    id:m_page
    width: 1000
    height: 250
    ListView{
        id:m_listView
        anchors.fill: parent
        model: 2
        /*同步index*/
        highlightRangeMode: ListView.StrictlyEnforceRange
        snapMode: ListView.SnapOneItem
        orientation:ListView.Horizontal
        delegate: Rectangle{
              width: m_listView.width
              height: m_listView.height
              Image {
                  width: m_page.width
                  height: m_page.height
                  source: index%2?"pageViewPic1.jpg":"pageViewPic2.jpg"
              }
        }
    }
    Rectangle{
        id:zroe
        x: 515
        y: 225
        width: 5
        height: 5
        radius: 3
        color: m_listView.currentIndex == 0? "white":"grey"
    }
    Rectangle{
        id: one
        x: 530
        y: 225
        width: 5
        height: 5
        radius: 3
        color: m_listView.currentIndex == 1? "white":"grey"
    }

    Timer{
        id: m_timer
        interval: 6000
        repeat: true
        running: true
        onTriggered: {

            if(m_listView.currentIndex == 0)
            {
               m_listView.currentIndex =1;
            }
            else
            {
               m_listView.currentIndex = 0;
            }
        }
    }

}
