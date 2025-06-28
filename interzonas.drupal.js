(function ($, Drupal, drupalSettings) { // Added Drupal and drupalSettings

  Drupal.behaviors.interzonasMapPage = { // Renamed behavior for clarity
    attach: function (context, settings) { // settings parameter is D7, use drupalSettings directly
      // Use once() to ensure the map is initialized only once per element.
      $('#mapa', context).once('interzonas-map-page').each(function () {
        // Get map data from drupalSettings.
        // These settings are populated in MapaController.
        const interzonasSettings = drupalSettings.interzonas;

        if (!interzonasSettings) {
          console.error('Interzonas settings not found.');
          return;
        }

        // Default view if not provided by settings (though it should be)
        let initialView = [51.495, -0.075];
        let initialZoom = 14;
        // Potentially, campo_mapa could store initial view and zoom.
        // For now, using static defaults if not in settings.

        // The element ID is 'mapa' from mapa.html.twig
        var map = L.map(this.id, { maxZoom: 17 }).setView(initialView, initialZoom);

        var imageUrl = interzonasSettings.campo_img || '/sites/default/files/TM_pano.jpg'; // Fallback, but should come from settings
        // Image bounds might also need to be dynamic if the image changes.
        // These are the original static bounds. If the map data includes bounds, use those.
        var imageBounds = [[51.490, -0.122], [51.510, -0.028]];

        if (interzonasSettings.image_bounds) { // Example if bounds were passed in settings
            imageBounds = interzonasSettings.image_bounds;
        }

        L.imageOverlay(imageUrl, imageBounds).addTo(map);

        var popup = L.popup();
        function onMapClick(e) {
          popup
            .setLatLng(e.latlng)
            .setContent("You clicked the map at " + e.latlng.toString()) // Updated message slightly
            .openOn(map);
        }
        map.on('click', onMapClick);
        map.setMaxBounds(imageBounds); // Restrict map panning to image bounds

        // Add markers if points are provided
        if (interzonasSettings.puntos && typeof interzonasSettings.puntos === 'object') {
          $.each(interzonasSettings.puntos, function (index, value) {
            if (value.lat && value.long) { // Ensure lat and long exist
              // Note: Original code had L.marker([value.long, value.lat]). Longitude is usually the second parameter.
              // Leaflet standard is [latitude, longitude]. Assuming 'long' was intended as latitude for this data.
              // If 'long' truly means longitude, it should be value.lat, value.long
              L.marker([parseFloat(value.long), parseFloat(value.lat)]).addTo(map) // Assuming long is lat, lat is lon from original
                .bindPopup('<h1>Marker: ' + index + '</h1><p>Data: ' + JSON.stringify(value) + '</p>'); // More informative popup
            }
          });
        }
      });
    }
  };
})(jQuery, Drupal, drupalSettings); // Pass Drupal and drupalSettings
