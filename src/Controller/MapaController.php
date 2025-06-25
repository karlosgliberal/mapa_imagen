<?php

namespace Drupal\interzonas\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\File\FileSystemInterface;
use Drupal\Core\Routing\RouteMatchInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Controller for the Interzonas map page.
 */
class MapaController extends ControllerBase {

  /**
   * The entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * The file system service.
   *
   * @var \Drupal\Core\File\FileSystemInterface
   */
  protected $fileSystem;

  /**
   * The current route match.
   *
   * @var \Drupal\Core\Routing\RouteMatchInterface
   */
  protected $routeMatch;

  /**
   * Constructs a new MapaController object.
   *
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   The entity type manager.
   * @param \Drupal\Core\File\FileSystemInterface $file_system
   *   The file system service.
   * @param \Drupal\Core\Routing\RouteMatchInterface $route_match
   *   The current route match.
   */
  public function __construct(EntityTypeManagerInterface $entity_type_manager, FileSystemInterface $file_system, RouteMatchInterface $route_match) {
    $this->entityTypeManager = $entity_type_manager;
    $this->fileSystem = $file_system;
    $this->routeMatch = $route_match;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('entity_type.manager'),
      $container->get('file_system'),
      $container->get('current_route_match')
    );
  }

  /**
   * Displays the map page.
   *
   * @return array
   *   A render array for the map page.
   */
  public function mapaPage() {
    $output = [];
    // Logic from interzonas_mapa_page_callback() and parts of hook_init()
    // and theme_mapa() will be migrated here or to related services/hooks.

    // Placeholder for loading node 1 data (equivalent to node_load(1)).
    $node = $this->entityTypeManager->getStorage('node')->load(1);
    $datos_mapa = new \stdClass();

    if ($node) {
      // Note: Accessing field values directly like this is D7 style.
      // In D10, we'd use $node->get('field_mapa')->getValue() or similar,
      // and ensure the field exists and has data.
      // This will need to be updated when field plugins are created.
      $datos_mapa->mapa = isset($node->field_mapa[0]) ? $node->field_mapa[0]->getValue() : NULL; // Placeholder
      $datos_mapa->img = isset($node->field_imagen[0]) ? $node->field_imagen[0]->entity->getFileUri() : NULL; // Placeholder

      // Attaching JavaScript settings (formerly drupal_add_js(..., 'setting')).
      // This needs to be more robust, checking field existence and values.
      $js_settings = [];
      if ($datos_mapa->mapa) {
        $js_settings['campo_mapa'] = $datos_mapa->mapa;
      }
      if ($datos_mapa->img) {
        // Generate URL from URI.
        $image_uri = $this->entityTypeManager->getStorage('file')->load($datos_mapa->img_fid_placeholder)->getFileUri(); // This needs actual FID
        $js_settings['campo_img'] = \Drupal::service('file_url_generator')->generateAbsoluteString($image_uri);

      }
       $puntos = [
        'punto1' => ['long' => '51.490,', 'lat' => '-0.075'],
        'punto2' => ['long' => '51.491,', 'lat' => '-0.074'],
      ];
      $js_settings['puntos'] = $puntos;

      $output['#attached']['drupalSettings']['interzonas'] = $js_settings;
    }


    // The theme hook 'mapa' will be defined in interzonas_theme().
    // The actual HTML structure (div#mapa) will be in the Twig template.
    $output['map_container'] = [
      '#theme' => 'mapa',
      '#datos_mapa' => $datos_mapa, // This will be passed to the Twig template.
    ];

    // Attach libraries defined in interzonas.libraries.yml
    $output['#attached']['library'][] = 'interzonas/leaflet';
    $output['#attached']['library'][] = 'interzonas/interzonas-styles'; // For CSS
    $output['#attached']['library'][] = 'interzonas/interzonas-js';   // For interzonas.drupal.js

    return $output;
  }

}
