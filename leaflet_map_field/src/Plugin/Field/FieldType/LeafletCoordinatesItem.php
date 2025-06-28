<?php

namespace Drupal\leaflet_map_field\Plugin\Field\FieldType;

use Drupal\Core\Field\FieldItemBase;
use Drupal\Core\Field\FieldStorageDefinitionInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\Core\TypedData\DataDefinition;
use Drupal\Core\TypedData\TraversableTypedDataInterface;

/**
 * Plugin implementation of the 'leaflet_coordinates_data' field type.
 *
 * @FieldType(
 *   id = "leaflet_coordinates_data",
 *   label = @Translation("Leaflet field"),
 *   description = @Translation("Stores coordinates from a Leaflet interactive map."),
 *   default_widget = "leaflet_field_coordinates_values",
 *   default_formatter = "leaflet_coordinates_default_formatter",
 *   constraints = {"LeafletCoordinatesFormat" = {}}
 * )
 */
class LeafletCoordinatesItem extends FieldItemBase {

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
      ->setLabel(new TranslatableMarkup('Coordinates'))
      ->setRequired(FALSE); // Or TRUE if coordinates are always required

    return $properties;
  }

  /**
   * {@inheritdoc}
   * Originally from leaflet_map_field_field_schema() in .install
   * Column name was 'leaflet_coordinates', here it's 'value'.
   */
  public static function schema(FieldStorageDefinitionInterface $field_definition) {
    return [
      'columns' => [
        'value' => [
          'type' => 'text', // D7 'text' type. Can store a reasonable amount of string data.
          'size' => 'normal', // For MySQL, 'normal' means TEXT.
          'not null' => FALSE,
        ],
      ],
    ];
  }

  /**
   * {@inheritdoc}
   * Originally from leaflet_map_field_field_settings_form() in .field.inc
   */
  public function fieldSettingsForm(array $form, FormStateInterface $form_state) {
    $element = [];
    $settings = $this->getSettings();

    // Using the same setting name as in interzonas module for consistency, if applicable.
    $element['default_map_image_fid'] = [
      '#type' => 'managed_file',
      '#title' => $this->t('Default map image'),
      '#description' => $this->t('Add the default background image/map for this field. This image will be shown on the map widget.'),
      '#default_value' => $settings['default_map_image_fid'] ? [$settings['default_map_image_fid']] : [],
      '#upload_location' => 'public://leaflet_map_field_images/', // Needs to be writable
      '#upload_validators' => [
        'file_validate_extensions' => ['png gif jpg jpeg'],
      ],
      '#multiple' => FALSE,
    ];

    return $element;
  }

  /**
   * {@inheritdoc}
   * Originally from leaflet_map_field_field_is_empty() in .field.inc
   * It checked for empTy($item['leaflet_coordinates']).
   */
  public function isEmpty() {
    $value = $this->get('value')->getValue();
    return $value === NULL || $value === '';
  }

  /**
   * {@inheritdoc}
   * For hook_field_validate, we use constraints.
   * The constraint "LeafletCoordinatesFormat" will be defined in a separate file.
   */
   // The constraint annotation is already added to the class.
   // The actual constraint plugin needs to be created (e.g., src/Plugin/Validation/Constraint/LeafletCoordinatesFormatConstraint.php and LeafletCoordinatesFormatConstraintValidator.php)
   // For now, this is a placeholder for where that validation logic connects.

}
