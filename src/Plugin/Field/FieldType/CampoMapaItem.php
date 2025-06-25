<?php

namespace Drupal\interzonas\Plugin\Field\FieldType;

use Drupal\Core\Field\FieldItemBase;
use Drupal\Core\Field\FieldStorageDefinitionInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\Core\TypedData\DataDefinition;

/**
 * Plugin implementation of the 'campo_mapa' field type.
 *
 * @FieldType(
 *   id = "campo_mapa",
 *   label = @Translation("Map field"),
 *   description = @Translation("Stores map data, potentially coordinates or configuration."),
 *   default_widget = "mapa_widget",
 *   default_formatter = "mapa_formatter"
 * )
 */
class CampoMapaItem extends FieldItemBase {

  /**
   * {@inheritdoc}
   */
  public static function defaultFieldSettings() {
    return [
      'default_map_image_fid' => '', // Stores the FID of the default map image
    ] + parent::defaultFieldSettings();
  }

  /**
   * {@inheritdoc}
   */
  public static function propertyDefinitions(FieldStorageDefinitionInterface $field_definition) {
    $properties['value'] = DataDefinition::create('string')
      ->setLabel(new TranslatableMarkup('Map data value'))
      ->setRequired(FALSE);

    return $properties;
  }

  /**
   * {@inheritdoc}
   */
  public static function schema(FieldStorageDefinitionInterface $field_definition) {
    return [
      'columns' => [
        'value' => [
          'type' => 'text',
          'size' => 'normal',
          'not null' => FALSE,
        ],
      ],
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function fieldSettingsForm(array $form, FormStateInterface $form_state) {
    $element = [];
    $settings = $this->getSettings();

    $element['default_map_image_fid'] = [
      '#type' => 'managed_file',
      '#title' => $this->t('Default map image'),
      '#description' => $this->t('Add the default image/map for this field type. This image can be used by widgets/formatters if no specific map data is entered.'),
      '#default_value' => $settings['default_map_image_fid'] ? [$settings['default_map_image_fid']] : [],
      '#upload_location' => 'public://interzonas_map_images/', // D10: ensure this directory exists and is writable.
      '#upload_validators' => [
        'file_validate_extensions' => ['png gif jpg jpeg'],
      ],
      '#multiple' => FALSE, // Ensure only one file can be uploaded
    ];
    // Note: The original code used $field['settings'][$field['field_name']].
    // Here, settings are directly keyed, e.g., 'default_map_image_fid'.
    // The $field['field_name'] part is not used as a key for the setting itself.

    return $element;
  }

  /**
   * {@inheritdoc}
   *
   * The original hook_field_is_empty checked for $item['campo_mapa'].
   * In D10 FieldItemBase, $this->values contains the properties.
   * We defined 'value' as our main property.
   */
  public function isEmpty() {
    $value = $this->get('value')->getValue();
    return $value === NULL || $value === '';
  }

}
