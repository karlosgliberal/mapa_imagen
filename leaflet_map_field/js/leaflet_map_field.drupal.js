(function ($, Drupal, drupalSettings) {

  Drupal.behaviors.leafletMapFieldInit = {
    attach: function (context, settings) { // settings is D7, use drupalSettings

      // Initialize Widgets
      if (drupalSettings.leafletMapFieldWidgets) {
        for (const mapId in drupalSettings.leafletMapFieldWidgets) {
          if (drupalSettings.leafletMapFieldWidgets.hasOwnProperty(mapId)) {
            const widgetSettings = drupalSettings.leafletMapFieldWidgets[mapId];
            const $mapElement = $(context).find('#' + widgetSettings.mapId);

            $mapElement.once('leaflet-map-widget-processed').each(function () {
              var map = L.map(widgetSettings.mapId, { maxZoom: 17 })
                .setView([widgetSettings.defaultView.lat, widgetSettings.defaultView.lon], widgetSettings.defaultView.zoom);

              var marker;
              var imageUrl = widgetSettings.defaultImageUrl;
              var imageBounds = widgetSettings.defaultBounds; // e.g., [[51.490, -0.122], [51.510, -0.028]]

              if (imageUrl) {
                L.imageOverlay(imageUrl, imageBounds).addTo(map);
              }
              map.setMaxBounds(imageBounds); // Restrict map panning

              // Function to update hidden input and marker
              function updateMapAndInput(lat, lng, updateInput) {
                if (marker) {
                  map.removeLayer(marker);
                }
                marker = L.marker([lat, lng]);
                map.addLayer(marker);
                if (updateInput) {
                  // Find the input field associated with this map widget
                  // The selector is passed in widgetSettings.inputSelector
                  // It's relative to the form.
                  $(this).closest('form').find(widgetSettings.inputSelector).val(lat + ', ' + lng);
                }
              }

              // Map click behavior for widget
              map.on('click', function(e) {
                updateMapAndInput.call($mapElement[0], e.latlng.lat, e.latlng.lng, true);
              });

              // Initial value from hidden input
              // The input selector needs to be relative to the current widget context,
              // not a global one like '.leaf-map-field'.
              // Let's assume the input field is discoverable from $mapElement.
              // The LeafletCoordinatesWidget passes 'inputSelector' in drupalSettings.
              const $inputField = $mapElement.closest('.js-form-item').find(widgetSettings.inputSelector);
              // If not found that way, try a broader search within the form if $mapElement is a child of form.
              // const $inputField = $(this).closest('form').find(widgetSettings.inputSelector);


              if ($inputField.length) {
                var punto = $inputField.val();
                if (punto && typeof punto === 'string' && punto.includes(',')) {
                  var latlngStr = punto.split(',');
                  if (latlngStr.length === 2) {
                    var initialLat = parseFloat(latlngStr[0].trim());
                    var initialLng = parseFloat(latlngStr[1].trim());
                    if (!isNaN(initialLat) && !isNaN(initialLng)) {
                      updateMapAndInput.call($mapElement[0], initialLat, initialLng, false); // Don't update input, just map
                      map.setView([initialLat, initialLng], widgetSettings.defaultView.zoom); // Center map on existing point
                    }
                  }
                }
              } else {
                console.warn('Leaflet Map Field Widget: Input field not found with selector: ' + widgetSettings.inputSelector);
              }
            });
          }
        }
      }

      // Initialize Formatters
      if (drupalSettings.leafletMapFieldFormatters) {
        for (const mapId in drupalSettings.leafletMapFieldFormatters) {
          if (drupalSettings.leafletMapFieldFormatters.hasOwnProperty(mapId)) {
            const formatterSettings = drupalSettings.leafletMapFieldFormatters[mapId];
            const $mapElement = $(context).find('#' + formatterSettings.mapId);

            $mapElement.once('leaflet-map-formatter-processed').each(function () {
              var map = L.map(formatterSettings.mapId, {
                maxZoom: 17, // Or from settings
                zoomControl: true, // Example: enable/disable based on settings
                scrollWheelZoom: false // Example: disable scroll wheel zoom for formatters
              });

              var imageUrl = formatterSettings.defaultImageUrl;
              var imageBounds = formatterSettings.defaultBounds; // e.g., [[51.490, -0.122], [51.510, -0.028]]

              if (imageUrl) {
                L.imageOverlay(imageUrl, imageBounds).addTo(map);
              }
               map.setMaxBounds(imageBounds);


              var initialLat, initialLng;
              var viewLat = formatterSettings.defaultBounds[0][0] + (formatterSettings.defaultBounds[1][0] - formatterSettings.defaultBounds[0][0]) / 2;
              var viewLng = formatterSettings.defaultBounds[0][1] + (formatterSettings.defaultBounds[1][1] - formatterSettings.defaultBounds[0][1]) / 2;


              if (formatterSettings.coordinates && typeof formatterSettings.coordinates === 'string' && formatterSettings.coordinates.includes(',')) {
                var latlngStr = formatterSettings.coordinates.split(',');
                 if (latlngStr.length === 2) {
                    initialLat = parseFloat(latlngStr[0].trim());
                    initialLng = parseFloat(latlngStr[1].trim());
                    if (!isNaN(initialLat) && !isNaN(initialLng)) {
                        viewLat = initialLat;
                        viewLng = initialLng;
                        if(formatterSettings.showMarker){
                             L.marker([initialLat, initialLng]).addTo(map);
                        }
                    }
                }
              }
              map.setView([viewLat, viewLng], formatterSettings.zoomLevel);
            });
          }
        }
      }
    }
  };

})(jQuery, Drupal, drupalSettings);
