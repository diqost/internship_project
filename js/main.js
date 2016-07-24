window.onload = function() {
    // TODO:: Do your initialization job

    // add eventListener for tizenhwkey
    document.addEventListener('tizenhwkey', function(e) {
        if (e.keyName === "back") {
            try {
                tizen.application.getCurrentApplication().exit();
            } catch (ignore) {}
        }
    });
};
document.addEventListener("rotarydetent", function(ev) {
	var dir = ev.detail.direction;
	if (dir === "CW") {
		map.zoomIn();

	} else if (dir === "CCW") {

		map.zoomOut();

	} else {
		alert("SMTH WRONG");
	}
});