var page = document.querySelector("#mainPage");
var handler = document.querySelector("#menuButton");
console.log(handler);
var STATE = {
		MAIN_MAP : 1,
		SHOWING_ROUTE : 2
};
var AppState = STATE.MAIN_MAP;

var menuPage = document.querySelector("#menuPage"), selector = document
		.getElementById("menuSelector"), clickBound, routesPopup = document
		.querySelector("#routesPopup"), routesMain = document
		.querySelector("#routesMain");
/**
 * pagebeforeshow event handler Do preparatory works and adds event listeners
 */

function menuButtonClickedHandler() {
	console.log("menu clicked");
	document.removeEventListener("rotarydetent", mapRotaryListener);

	tau.changePage(menuPage);
}
function menuItemClickedHandler(event) {
	var target = event.target;

	if (target.classList.contains("ui-selector-indicator"))
		menuItemWasSelected(parseInt($("#menuSelector > [data-index]").attr(
				"data-index")));
	console.log("clicked menu item");
}

function mapRotaryListener(ev) {
	var dir = ev.detail.direction;
	if (dir === "CW") {
		map.zoomIn();

	} else if (dir === "CCW") {

		map.zoomOut();

	} else {
		alert("SMTH WRONG");
	}
}

handler.addEventListener("click", menuButtonClickedHandler);
page.addEventListener( "pagebeforeshow",function() {
	console.log("showing MAIN PAGE");
	
	document.addEventListener("rotarydetent", mapRotaryListener);
	if(AppState === STATE.MAIN_MAP)
		$("#goFooter").css({"display":"none"});
	else 
		$("#goFooter").css({"display":"block"});
});
page.addEventListener( "pagebeforehide",function() {
	console.log("HIDING MAIN PAGE");
	document.removeEventListener("rotarydetent", mapRotaryListener);
});


menuPage.addEventListener("pagebeforeshow", function() {

	clickBound = menuItemClickedHandler.bind(null);
	tau.widget.Selector(selector);
	selector.addEventListener("click", clickBound, false);

	/*
	 * var radius = window.innerHeight / 2 * 0.8;
	 * 
	 * clickHandlerBound = menuClickedHandler.bind(null);
	 * handler.addEventListener("click", clickHandlerBound); if
	 * (tau.support.shape.circle) { selector = tau.widget.Selector(elSelector, {
	 * itemRadius : radius }); }
	 */
});
menuPage.addEventListener("pagebeforehide", function() {
	selector.removeEventListener("click", clickBound, false);

	/*
	 * handler.removeEventListener("click", clickHandlerBound); if
	 * (tau.support.shape.circle) { selector.destroy(); }
	 */
});
routesMain.addEventListener("pagebeforeshow", function() {
	console.log("openning page routes main");
});
routesMain.addEventListener("pagebeforehide", function() {
	console.log("hiding page routes main");
});

function closeMenu() {
	console.log("closing menu");
	tau.changePage(mainPage);
}
var MENU_ITEMS = {
	BACK : 0,
	SEARCH : 1,
	ROUTES : 2
};
function menuItemWasSelected(itemIndex) {
	switch (itemIndex) {
	case MENU_ITEMS.BACK:
		closeMenu();
		break;
	case MENU_ITEMS.SEARCH:
		break;
	case MENU_ITEMS.ROUTES:
		AppState = STATE.SHOWING_ROUTE;
		tau.changePage("#routesMain");
		// tau.openPopup(routesPopup);

		break;
	}
	console.log(itemIndex);
}
