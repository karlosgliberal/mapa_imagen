<?php

namespace Drupal\interzonas\Plugin\Field\FieldWidget;

use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Field\WidgetBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;
use Drupal\file\Entity\File;

/**
 * Plugin implementation of the 'mapa_widget' widget.
 *
 * @FieldWidget(
 *   id = "mapa_widget",
 *   label = @Translation("Mapa Widget"),
 *   field_types = {
 *     "campo_mapa"
 *   }
 * )
 */
class MapaWidget extends WidgetBase {

  /**
   * {@inheritdoc}
   */
  public function formElement(FieldItemListInterface $items, $delta, array $element, array &$form, FormStateInterface $form_state) {
    $value = isset($items[$delta]->value) ? $items[$delta]->value : '';

    // Get the default map image FID from field settings.
    $field_settings = $this->fieldDefinition->getSettings();
    $default_image_fid = $field_settings['default_map_image_fid'] ?? NULL;
    $image_url = '';

    if (!empty($default_image_fid) && is_array($default_image_fid)) {
        // The managed_file element in field settings form returns an array of fids.
        $default_image_fid = reset($default_image_fid);
    }


    if ($default_image_fid) {
      $file = File::load($default_image_fid);
      if ($file) {
        $image_url = \Drupal::service('file_url_generator')->generateAbsoluteString($file->getFileUri());
      }
    }

    // Main element for the field value (hidden input)
    $element['value'] = $element + [
      '#type' => 'hidden',
      '#default_value' => $value,
      '#attributes' => ['class' => ['interzonas-mapa-value']], // Class for JS to target
    ];

    // Visual representation of the map for interaction
    // The suffix was '<div id="mapa" style="width: 700px; height: 500px; position: relative;"></div>'
    // We can make this more dynamic, perhaps allowing ID to be unique if multiple widgets are on page.
    $map_id = 'mapa-' . $items->getName() . '-' . $delta;
    $element['map_display'] = [
      '#type' => 'markup',
      '#markup' => '<div id="' . $map_id . '" class="interzonas-map-widget-display" style="width: 700px; height: 500px; position: relative;"></div>',
      '#allowed_tags' => ['div'],
    ];

    // Attach the library for the widget's JavaScript and Leaflet.
    // Also pass the map_id and default image URL to JavaScript via drupalSettings.
    $element['#attached']['library'][] = 'interzonas/interzonas-field-js';
    $element['#attached']['drupalSettings']['interzonas']['mapWidget'] = [
        $map_id => [ // Key by map_id to support multiple widgets
            'defaultImageUrl' => $image_url,
            'fieldValueSelector' => '.interzonas-mapa-value', // Selector for JS to find the hidden input, might need to be more specific if multiple widgets
        ]
    ];
     // Ensure the drupalSettings for this specific widget are namespaced correctly.
    // The JavaScript will need to find its settings using this map_id.
    // For example, in JS: drupalSettings.interzonas.mapWidget[mapIdFromDiv]

    // The original D7 code used drupal_add_js( array('park_map' => $image_path), 'setting');
    // This is now passed via drupalSettings above.
    // The 'park_map' key might have been specific to that JS file's expectation.
    // The interzonas_field.drupal.js will need to be updated to use these new settings.

    return $element;
  }
}
