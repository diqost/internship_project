ymaps.ready(init);
	var map, myPlacemark;

	function init() {
		map = new ymaps.Map("map", {
			center : [ 55.76, 37.64 ],
			zoom : 7
		});

		myPlacemark = new ymaps.Placemark([ 55.76, 37.64 ], {
			hintContent : 'Москва!',
			balloonContent : 'Столица России'
		});
		
 		console.log(map);
		//map._zoom = Math.max(map.zoom + 1, 17);

	}