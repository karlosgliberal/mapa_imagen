<?php

namespace Drupal\leaflet_map_field\Plugin\Field\FieldWidget;

use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Field\WidgetBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;
use Drupal\file\Entity\File;

/**
 * Plugin implementation of the 'leaflet_field_coordinates_values' widget.
 *
 * @FieldWidget(
 *   id = "leaflet_field_coordinates_values",
 *   label = @Translation("Leaflet Coordinates field widget"),
 *   field_types = {
 *     "leaflet_coordinates_data"
 *   }
 * )
 */
class LeafletCoordinatesWidget extends WidgetBase {

  /**
   * {@inheritdoc}
   */
  public function formElement(FieldItemListInterface $items, $delta, array $element, array &$form, FormStateInterface $form_state) {
    $value = isset($items[$delta]->value) ? $items[$delta]->value : '';

    // Get the default map image FID from field settings.
    $field_settings = $this->fieldDefinition->getSettings();
    $default_image_fid_array = $field_settings['default_map_image_fid'] ?? [];
    $default_image_fid = is_array($default_image_fid_array) ? reset($default_image_fid_array) : $default_image_fid_array;

    $image_url = '';
    if ($default_image_fid) {
      $file = File::load($default_image_fid);
      if ($file) {
        // It's better to pass the URI and let the JS request the URL,
        // or ensure the URL generated is appropriate for the context (e.g. relative or absolute).
        // For drupalSettings, an absolute URL is often safer if JS can't resolve relative paths easily.
        $image_url = \Drupal::service('file_url_generator')->generateAbsoluteString($file->getFileUri());
      }
    }

    // The main element for the field value (hidden input).
    // Class 'leaf-map-field' was used in D7 JS.
    $element['value'] = $element + [
      '#type' => 'hidden',
      '#default_value' => $value,
      '#attributes' => ['class' => ['leaflet-map-field-coordinates-value']], // New class for D10
    ];

    // Visual representation of the map for interaction.
    // The D7 suffix was '<div id="field_map" style="width: 700px; height: 500px; position: relative;"></div>'
    // We need a unique ID if multiple widgets are on the page.
    $map_id = 'leaflet-map-field-' . $items->getName() . '-' . $delta;

    $element['map_display'] = [
      '#type' => 'markup',
      '#markup' => '<div id="' . $map_id . '" class="leaflet-map-field-widget-display" style="width: 700px; height: 500px; position: relative;"></div>',
      '#allowed_tags' => ['div'], // Kint includes div by default.
    ];

    // Attach the library for the widget's JavaScript.
    // Pass necessary data to JavaScript via drupalSettings.
    $settings_for_js = [
      'mapId' => $map_id,
      'defaultImageUrl' => $image_url,
      'inputSelector' => '.' . 'leaflet-map-field-coordinates-value', // Class of the hidden input
      // Initial lat/lon for setView if no value is present, can be taken from D7 JS or made configurable.
      'defaultView' => [
          'lat' => 51.495,
          'lon' => -0.075,
          'zoom' => 14,
      ],
      // Default bounds if not derived from image or other settings.
      'defaultBounds' => [[51.490, -0.122], [51.510, -0.028]], // From D7 JS
    ];

    // The D7 JS also had Drupal.settings.leaflet_map_field.lat and .lon
    // These seem to be static. They could be part of field instance settings if they need to vary.
    // For now, adding them to the settings for this widget instance.
    // The JS used these for setView if no value was present in the field.
    // $settings_for_js['lat'] = 51.495; // Example, if these were global.
    // $settings_for_js['lon'] = -0.075;

    $element['#attached']['library'][] = 'leaflet_map_field/leaflet-map-field-widget';
    $element['#attached']['drupalSettings']['leafletMapFieldWidgets'][$map_id] = $settings_for_js;

    return $element;
  }
}
