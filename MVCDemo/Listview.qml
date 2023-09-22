import QtQuick 2.7
import QtQuick.Controls 2.2
//import ListModels 1.0

Rectangle
{
    //width: 1920
    //height: 720
    Image
    {
        anchors.fill: parent;
        source: "file:///d:/1.png"
    }

    GridView
        {
            id:addressBookView;
            model: listModels;
            //model: addressBookModel;
            delegate: addressBookDelegate;
            anchors.fill: parent;
            cellWidth: 480;
            cellHeight: 360;
            snapMode: GridView.SnapToRow
            flow: GridView.FlowTopToBottom;
            //上次点击的index
            property int lastIndex: -1;
            //上次点击对应的item
            property var lastItem: null;
            //增加滑动条
            ScrollBar.vertical: ScrollBar
            {
                id: scrollBar;
                visible: false;
                policy: ScrollBar.AlwaysOn;
            }
            ScrollBar.horizontal: ScrollBar
            {
                id:hscrollBar;
                visible:false;
                policy:ScrollBar.AsNeeded
            }

        }

        //列表委托
        Component
        {
            id:addressBookDelegate;
            Rectangle
            {
                id: singleItemInView;
                width: 480;
                height: 360;
                color: "transparent"
                //var tmp=Math.round(Math.random());
                MouseArea
                {
                    anchors.fill: parent;
                    onClicked:
                    {
                        console.log(suibian);
                    }
                }

                Rectangle
                {
                    id: blueRect;
                    width: 240;
                    height: 160;
                    color: "blue";
                    anchors.left: parent.left;
                    anchors.leftMargin: 120;
                    anchors.top: parent.top;
                    anchors.topMargin: 100;
                }

                //头像区域
                Rectangle
                {
                    id:imageLabel;
                    width: 50;
                    height: 50;
                    anchors.top: parent.top;
                    anchors.topMargin: 55;
                    anchors.left: parent.left;
                    anchors.leftMargin: 215;
                    Image
                    {
                        id: profileImage
                        anchors.fill: parent;
                        source: imagePath;
                    }
                }
                //名字区域
                Rectangle
                {
                    id:nameLabel;
                    color: "transparent";
                    width: 180;
                    height: 30;
                    anchors.top: parent.top;
                    anchors.topMargin: 30;
                    anchors.left: imageLabel.right;
                    anchors.leftMargin: 15;
                    Text
                    {
                        id: nameText;
                        elide: Text.ElideRight;
                        anchors.fill: parent;
                        font.family: "msyh";
                        font.pixelSize: 24;
                        color: "black";
                        text: name;
                    }
                }

            }
        }
}
