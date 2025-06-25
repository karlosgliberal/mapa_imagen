<?php

namespace Drupal\leaflet_map_field\Plugin\Field\FieldFormatter;

use Drupal\Core\Field\FormatterBase;
use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\file\Entity\File;
use Drupal\Core\Url;

/**
 * Plugin implementation of the 'leaflet_coordinates_default_formatter' formatter.
 *
 * @FieldFormatter(
 *   id = "leaflet_coordinates_default_formatter",
 *   label = @Translation("Leaflet Coordinates Default Formatter"),
 *   field_types = {
 *     "leaflet_coordinates_data"
 *   }
 * )
 */
class LeafletCoordinatesFormatter extends FormatterBase {

  /**
   * {@inheritdoc}
   */
  public static function defaultSettings() {
    // Add any settings specific to the formatter here.
    // For example, map dimensions, zoom level for display, etc.
    return [
      'map_width' => '100%',
      'map_height' => '400px',
      'zoom_level' => 13,
      'show_marker' => TRUE,
      'show_image_overlay' => TRUE,
    ] + parent::defaultSettings();
  }

  /**
   * {@inheritdoc}
   */
  public function settingsSummary() {
    $summary = [];
    $summary[] = $this->t('Displays the map with the stored coordinates.');
    $summary[] = $this->t('Map dimensions: @width x @height.', ['@width' => $this->getSetting('map_width'), '@height' => $this->getSetting('map_height')]);
    if ($this->getSetting('show_marker')) {
      $summary[] = $this->t('Marker is shown.');
    }
    if ($this->getSetting('show_image_overlay')) {
      $summary[] = $this->t('Image overlay is shown (if configured).');
    }
    return $summary;
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm(array $form, FormStateInterface $form_state) {
    $elements = parent::settingsForm($form, $form_state);

    $elements['map_width'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Map width'),
      '#default_value' => $this->getSetting('map_width'),
      '#description' => $this->t('Enter the width of the map (e.g., 100%, 600px).'),
      '#required' => TRUE,
    ];

    $elements['map_height'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Map height'),
      '#default_value' => $this->getSetting('map_height'),
      '#description' => $this->t('Enter the height of the map (e.g., 400px).'),
      '#required' => TRUE,
    ];

    $elements['zoom_level'] = [
      '#type' => 'number',
      '#title' => $this->t('Default Zoom Level'),
      '#default_value' => $this->getSetting('zoom_level'),
      '#min' => 0,
      '#max' => 18, // Max zoom for typical Leaflet maps
    ];

    $elements['show_marker'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Show marker for coordinates'),
      '#default_value' => $this->getSetting('show_marker'),
    ];

    $elements['show_image_overlay'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Show image overlay (if default image is set)'),
      '#default_value' => $this->getSetting('show_image_overlay'),
    ];

    return $elements;
  }

  /**
   * {@inheritdoc}
   */
  public function viewElements(FieldItemListInterface $items, $langcode) {
    $elements = [];
    $field_settings = $this->fieldDefinition->getSettings();
    $default_image_fid_array = $field_settings['default_map_image_fid'] ?? [];
    $default_image_fid = is_array($default_image_fid_array) ? reset($default_image_fid_array) : $default_image_fid_array;

    $default_image_url = '';
    if ($default_image_fid) {
      $file = File::load($default_image_fid);
      if ($file) {
        $default_image_url = \Drupal::service('file_url_generator')->generateAbsoluteString($file->getFileUri());
      }
    }

    foreach ($items as $delta => $item) {
      $coordinates_value = $item->value; // Format: "lat,lng" string
      $map_id = 'leaflet-map-formatter-' . $items->getName() . '-' . $delta . '-' . $this->viewMode;

      $js_settings = [
        'mapId' => $map_id,
        'coordinates' => $coordinates_value, // Pass the "lat,lng" string
        'defaultImageUrl' => $this->getSetting('show_image_overlay') ? $default_image_url : '',
        'mapWidth' => $this->getSetting('map_width'),
        'mapHeight' => $this->getSetting('map_height'),
        'zoomLevel' => $this->getSetting('zoom_level'),
        'showMarker' => $this->getSetting('show_marker'),
        // Default bounds, could be made configurable or derived from image if available
        'defaultBounds' => [[51.490, -0.122], [51.510, -0.028]],
      ];

      $elements[$delta] = [
        // Using a generic theme hook for map display, can be shared or specific.
        // Or directly output markup here. For flexibility, a theme hook is good.
        '#theme' => 'leaflet_map_field_formatter_map',
        '#map_id' => $map_id,
        '#map_width' => $this->getSetting('map_width'),
        '#map_height' => $this->getSetting('map_height'),
        '#attached' => [
          // A separate JS library for the formatter, or the widget JS if it can handle both modes.
          // For now, assuming a new library 'leaflet-map-field-formatter'.
          'library' => ['leaflet_map_field/leaflet-map-field-formatter'],
          'drupalSettings' => ['leafletMapFieldFormatters' => [$map_id => $js_settings]],
        ],
      ];
    }

    return $elements;
  }
}
