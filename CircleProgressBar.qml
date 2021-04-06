import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2
//import QtQuick.Controls 2.5
import QtQuick.Controls 2.0

import QtQuick.Controls.Styles 1.4

Rectangle{
  id: base

  property alias currentPercent: canvas.percent
  property alias text: label_text.text
  signal receFromWidget(int printRec)
  function setPercent(printRec)
  {
    canvas.percent = printRec;
    lable_percent.text = printRec + "%";
  }
  Component.onCompleted:
  {
      base.receFromWidget.connect(base.setPercent);
  }
  Component.onDestruction:
  {
      base.receFromWidget.disconnect(base.setPercent);
  }

  onCurrentPercentChanged: {
    canvas.requestPaint()
  }

  color: "#00000000"

  Canvas {
    id: canvas

    property int percent

    anchors.fill: parent

    onPaint : {
      var ctx = canvas.getContext('2d');

      ctx.beginPath();
      ctx.lineWidth = 2;
      ctx.fillStyle = "#202023";
      ctx.ellipse(2, 2, parent.width-4, parent.height-4);
      ctx.closePath();
      ctx.fill();
      
      ctx.beginPath();
      ctx.lineCap = "round"
      ctx.lineWidth = 58;
      var gradient = ctx.createConicalGradient(parent.width / 2, parent.height / 2, Math.PI/2)
      // gradient.addColorStop(, "#F3B67B")
      gradient.addColorStop(0, "#F3B67B")
      gradient.addColorStop(0.5, "#FF5B00")
      gradient.addColorStop(0.75, "#F3B67B")
      ctx.strokeStyle = gradient
      ctx.arc(parent.width / 2, parent.height / 2, parent.width / 2 - 29, -Math.PI / 2, -Math.PI / 2 + percent / 100* (Math.PI * 2), false);
      ctx.stroke();
      ctx.closePath();
      
    }
  }

  Label {
    id: lable_percent

    anchors.centerIn: parent
    font {pixelSize: 64}
    color: "white"
    text: currentPercent + "%"
  }

  Label {
    id: label_text
    anchors {top: lable_percent.bottom; horizontalCenter:parent.horizontalCenter; margins: 10}
    color: "white"
    font {
            pixelSize: 64;
            family: "Barlow";
    }
  }

}
