(function ($) {

  Drupal.behaviors.ParkMap = {
    attach:function (context, settings) {
      
      if($('#map').length) {

        var map = L.map('map',{maxZoom: 17}).setView([51.495, -0.075],14);
        var marker;
        var imageUrl = Drupal.settings.park_map.image_absolute_path;
        var imageBounds = [[51.490, -0.122], [51.510, -0.028]];

        L.imageOverlay(imageUrl, imageBounds).addTo(map);
      }

    }
  };
})(jQuery);