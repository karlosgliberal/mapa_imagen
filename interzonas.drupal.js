(function ($) {

  Drupal.behaviors.interzonas = {
    attach:function (context, settings) {
    var map = L.map('mapa', {maxZoom: 17}).setView([51.495, -0.075], 14);

    var imageUrl = '/sites/default/files/TM_pano.jpg';
    var imageBounds = [[51.490, -0.122], [51.510, -0.028]];
    L.imageOverlay(imageUrl, imageBounds).addTo(map);


    var popup = L.popup();
    function onMapClick(e) {
      popup
        .setLatLng(e.latlng)
        .setContent("has realizado click en el mapa" + e.latlng.toString())
        .openOn(map);
    }
    map.on('click', onMapClick);
    map.setMaxBounds(imageBounds);
    $.each(settings.interzonas.puntos, function(index, value){
      L.marker([value.long, value.lat]).addTo(map)
        .bindPopup('<h1>test html</h1>');
    });

    }
  };
})(jQuery);
