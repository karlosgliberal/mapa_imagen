<?php

namespace Drupal\leaflet_map_field\Plugin\Validation\Constraint;

use Symfony\Component\Validator\Constraint;
use Symfony\Component\Validator\ConstraintValidator;

/**
 * Validates the LeafletCoordinatesFormat constraint.
 */
class LeafletCoordinatesFormatConstraintValidator extends ConstraintValidator {

  /**
   * {@inheritdoc}
   */
  public function validate($value, Constraint $constraint) {
    // The field item value is $value->value.
    $field_value = $value->value;

    // Allow empty values, as per hook_field_is_empty and not null => FALSE in schema.
    // If the field is required, the NotNull or NotBlank constraint should be used.
    if ($field_value === NULL || $field_value === '') {
      return;
    }

    if (!is_string($field_value)) {
      $this->context->addViolation($constraint->notStringMessage, ['%value' => $this->formatValue($field_value)]);
      return;
    }

    // Additional validation logic for format (e.g., "lat,lng") can be added here.
    // For example, checking if it's a comma-separated pair of numbers.
    // $parts = explode(',', $field_value);
    // if (count($parts) !== 2 || !is_numeric(trim($parts[0])) || !is_numeric(trim($parts[1]))) {
    //   $this->context->addViolation($constraint->invalidFormatMessage, ['%value' => $field_value]);
    // }
  }

}
