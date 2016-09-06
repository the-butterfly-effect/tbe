import QtQuick 2.0

//import "gui"

Rectangle {
    id: gameView

    function img(A) {
        return "image://tbe/"+A;
    }

    border.color: "black"

    gradient: Gradient {
        GradientStop {
            position: 0.00;
            color: "#6baaf1";
        }
        GradientStop {
            position: 0.64;
            color: "#ffffff";
        }
        GradientStop {
            position: 0.98;
            color: "#175800";
        }
    }
}

// TODO: real content, for later
//Toolbox {
//    id: toolbox
//    width: 200
//    height: 800
//}
