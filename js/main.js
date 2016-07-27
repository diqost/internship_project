window.onload = function() {
	// TODO:: Do your initialization job

	// add eventListener for tizenhwkey
	window
			.addEventListener(
					'tizenhwkey',
					function(ev) {
						if (ev.keyName === "back") {
							var page = document
									.getElementsByClassName('ui-page-active')[0], pageid = page ? page.id
									: "";

							switch (pageid) {
							case "mainPage":
								console.log("pop from main, exiting");
								tizen.application.getCurrentApplication()
										.exit();
								break;
							case "menuPage":
								console.log("pop from menu to mainPage");
								tau.changePage("#mainPage");
								break;
							case "routesMain":
								console.log("pop from routes main to menuPage");
								tau.changePage("#menuPage");
								break;
							}
						}
					});

};
function googleInit() {
	console.log("google was inited");
}