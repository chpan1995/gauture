var component;
var tagPane;
var firstModel;
var secondModel;

function createTagPaneObjects(firstModel_,secondModel_) {
    component = Qt.createComponent("TagPane.qml");
    if (component.status == Component.Ready){
        firstModel=firstModel_;
        secondModel=secondModel_
        finishCreation();
    }
    else
        component.statusChanged.connect(finishCreation);
}

function finishCreation() {
    if (component.status == Component.Ready) {
        tagPane = component.createObject(stk, {firstModel: firstModel, secondModel: secondModel});
        if (tagPane == null) {
            // Error Handling
            console.log("Error creating object");
        }
    } else if (component.status == Component.Error) {
        // Error Handling
        console.log("Error loading component:", component.errorString());
    }
}
