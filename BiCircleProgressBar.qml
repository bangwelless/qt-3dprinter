import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
Rectangle{
  id: base

  property alias leftPercent: canvas.leftPercent
  property alias rightPercent: canvas.rightPercent


  signal receFromWidgetT(int l ,int r)
  function setPercent(l,r)
  {
    leftPercent = l;
    rightPercent = r;
  }

  Component.onCompleted:
  {
      base.receFromWidgetT.connect(base.setPercent);
  }
  onLeftPercentChanged: {
    canvas.requestPaint()
  }

  onRightPercentChanged: {
    canvas.requestPaint()
  }

  color: "#00000000"

  Canvas {
    id: canvas

    property int leftPercent
    property int rightPercent

    anchors.fill: parent

    onPaint : {
      var ctx = canvas.getContext('2d');
      ctx.clearRect(0, 0, parent.width, parent.height);

      ctx.beginPath();
      ctx.lineWidth = 2;
      ctx.fillStyle = "#202023";
      ctx.ellipse(1, 1, parent.width-2, parent.height-2);
      ctx.closePath();
      ctx.fill();

      // ctx.beginPath();
      // ctx.lineWidth = 2;
      // ctx.fillStyle = "#e0e0e0";
      // ctx.ellipse(32, 32, parent.width-16, parent.height-16);
      // ctx.closePath();
      // ctx.fill();

      ctx.lineCap = "round"
      ctx.lineWidth = 58;
      var gradient = ctx.createConicalGradient(parent.width / 2, parent.height / 2, Math.PI/2)
      gradient.addColorStop(0, "#F3B67B")
      gradient.addColorStop(0.5, "#FF5B00")
      gradient.addColorStop(0.75, "#F3B67B")
      ctx.strokeStyle = gradient

      ctx.beginPath();
      // ctx.arc(parent.width / 2, parent.height / 2, parent.width / 2 - 29, -Math.PI / 2, -Math.PI / 2 + percent / 100 * (Math.PI), false);
      ctx.arc(parent.width / 2, parent.height / 2, parent.width / 2 - 29, Math.PI / 2, Math.PI / 2 - rightPercent / 100 * (Math.PI), true);
      ctx.stroke();
      ctx.closePath();

      ctx.beginPath();
      ctx.arc(parent.width / 2, parent.height / 2, parent.width / 2 - 29, Math.PI / 2, Math.PI / 2 + leftPercent / 100 * (Math.PI), false);
      ctx.stroke();
      ctx.closePath();
    }
  }

  Label {
    id: lable_heating

    anchors.centerIn: parent
    font {
        pixelSize: 64;
        family: "Barlow";
    }
    color: "white"
    text: "Heating"
  }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
