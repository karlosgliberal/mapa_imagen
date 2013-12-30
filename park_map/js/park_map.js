(function ($) {

  Drupal.behaviors.ParkMap = {
    attach:function (context, settings) {
      
      if($('#map').length) {
        // var southWest = Drupal.settings.park_map.geolocation.southwet_llatlong.split(", ");
        // var northEast = Drupal.settings.park_map.geolocation.northeast_latlong.split(", ");
        // var bounds = L.latLngBounds(southWest, northEast); 

        var map = L.map('map',{
        crs:L.CRS.Simple
        ,minZoom:9
        ,maxZoom:12
        //,zoom:1
        
        });
        var marker;
        var imageUrl = Drupal.settings.park_map.image_absolute_path;
        // var imageUrl = '/sites/default/files/parkmap/media.jpg';


        var sP = map.unproject([0,3000],map.getMaxZoom());
        console.log(sP);
        var nP = map.unproject([6000,0],map.getMaxZoom());
        var imgBll = L.latLngBounds(sP,nP);
        map.setMaxBounds(imgBll);
        L.imageOverlay(imageUrl,[sP,nP] ).addTo(map);
        marker = L.marker([1000, -1000]);
        console.log(marker);
        map.addLayer(marker);

        map.fitBounds([sP,nP]);
        console.log(imgBll);
        // map.fitWorld();

        function onMapClick(e) {
            marker = L.marker([e.latlng.lat, e.latlng.lng]);
            map.addLayer(marker);
            console.log(marker);
        }
        map.on('click', onMapClick);

      }

    }
  };
})(jQuery);
