<?php

namespace Drupal\interzonas\Plugin\Field\FieldFormatter;

use Drupal\Core\Field\FormatterBase;
use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\file\Entity\File;
use Drupal\Core\Url;

/**
 * Plugin implementation of the 'mapa_formatter' formatter.
 *
 * @FieldFormatter(
 *   id = "mapa_formatter",
 *   label = @Translation("Mapa Formatter"),
 *   field_types = {
 *     "campo_mapa"
 *   }
 * )
 */
class MapaFormatter extends FormatterBase {

  /**
   * {@inheritdoc}
   */
  public static function defaultSettings() {
    return [
      'display_style' => 'interactive_map', // Example setting: 'interactive_map' or 'static_image'
    ] + parent::defaultSettings();
  }

  /**
   * {@inheritdoc}
   */
  public function settingsSummary() {
    $summary = [];
    $summary[] = $this->t('Displays the map.');
    // Could add more summary based on settings, e.g., $this->getSetting('display_style').
    return $summary;
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm(array $form, FormStateInterface $form_state) {
    $elements = parent::settingsForm($form, $form_state);
    // Example setting if we want to allow configuration on the formatter.
    /*
    $elements['display_style'] = [
      '#type' => 'select',
      '#title' => $this->t('Display style'),
      '#options' => [
        'interactive_map' => $this->t('Interactive Map'),
        'static_image' => $this->t('Static Image (Placeholder)'),
      ],
      '#default_value' => $this->getSetting('display_style'),
    ];
    */
    return $elements;
  }

  /**
   * {@inheritdoc}
   */
  public function viewElements(FieldItemListInterface $items, $langcode) {
    $elements = [];
    $field_settings = $this->fieldDefinition->getSettings();
    $default_image_fid = $field_settings['default_map_image_fid'] ?? NULL;
    $default_image_url = '';

    if (!empty($default_image_fid) && is_array($default_image_fid)) {
        $default_image_fid = reset($default_image_fid);
    }

    if ($default_image_fid) {
      $file = File::load($default_image_fid);
      if ($file) {
        $default_image_url = \Drupal::service('file_url_generator')->generateAbsoluteString($file->getFileUri());
      }
    }

    foreach ($items as $delta => $item) {
      $map_value = $item->value; // The actual stored value for the map.
      $map_id = 'mapa-formatter-' . $items->getName() . '-' . $delta . '-' . $this->viewMode;

      // Prepare settings for JavaScript.
      // The formatter JS might be different from widget JS.
      // For now, we'll assume interzonas.drupal.js or a new one handles formatted map display.
      $js_settings = [
        'mapId' => $map_id,
        'mapValue' => $map_value,
        'defaultImageUrl' => $default_image_url,
        // Add other necessary data for rendering, e.g., points from $map_value if it's structured data.
      ];

      $elements[$delta] = [
        '#theme' => 'mapa_formatter_display', // A new theme hook for the formatter
        '#map_id' => $map_id,
        '#map_value' => $map_value,
        '#default_image_url' => $default_image_url,
        '#attached' => [
          'library' => ['interzonas/interzonas-js'], // Or a dedicated formatter JS library
          'drupalSettings' => ['interzonas' => ['mapFormatter' => [$map_id => $js_settings]]],
        ],
      ];
    }

    return $elements;
  }
}
