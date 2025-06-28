<?php

namespace Drupal\leaflet_map_field\Plugin\Validation\Constraint;

use Symfony\Component\Validator\Constraint;

/**
 * Checks that the submitted value is a string, suitable for coordinates.
 *
 * @Constraint(
 *   id = "LeafletCoordinatesFormat",
 *   label = @Translation("Leaflet Coordinates Format", context = "Validation"),
 *   type = "string"
 * )
 */
class LeafletCoordinatesFormatConstraint extends Constraint {

  // The message that will be shown if the value is not a string.
  public $notStringMessage = 'Coordinates data must be a string. Your data seems to be wrong: %value';

  // The message for other types of invalid formats, if needed.
  // public $invalidFormatMessage = 'The coordinates "%value" are not in a recognized format (e.g., "lat,lng").';

}
