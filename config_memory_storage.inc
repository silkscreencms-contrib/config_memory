<?php

/**
 * Defines an interface for configuration storage controllers.
 *
 * Classes implementing this interface allow reading and writing configuration
 * data from and to the storage.
 */
class ConfigMemoryStorage implements ConfigStorageInterface {

  /**
   * The name of the storage class.
   * @var string
   */
  static $storage_class = 'Memory';

  protected $local_config = NULL;

  protected $created = array();

  protected $immutable = TRUE;

  public static function urlPrefix() {
    return 'mem';
  }

  public function isImmutable() {
    return $this->immutable;
  }

  /**
   * {@inheritdoc}
   */
  public function initializeStorage() {
    if (empty($this->local_config)) {
      $this->local_config = array();
      $this->created = array();
    }
  }

  /**
   * Accepts:
   *  mem:var_name
   *  mem:/var_name
   *  mem://var_name
   */
  public function __construct($url) {
    global $settings;

    $matches = array();
    $prefix = $this->urlPrefix();
    preg_match("/^({$prefix}):\/{0,2}(\w*)$/", $url, $matches);
    if (count($matches) != 3) {
      throw new ConfigStorageException(t('Invalid config specifier: @url', array('@url' => $url)));
    }
    $var_name = $matches[2];
    if (!array_key_exists('config', $settings) || !array_key_exists($var_name, $settings['config'])) {
      throw new ConfigStorageException(t('No config settings name @varname.', array('@varname' => $var_name)));
    }
    if (!is_array($settings['config'][$var_name])) {
      throw new ConfigStorageException(t('Config settings @varname is not an array.', array('@varname' => $var_name)));
    }
    $this->local_config = $settings['config'][$var_name];
    $this->created = array_fill_keys(array_keys($this->local_config), time());
  }

  /**
   * {@inheritdoc}
   */
  public function isInitialized() {
    return is_array($this->local_config);
  }

  /**
   * {@inheritdoc}
   */
  public function exists($name) {
    return array_key_exists($name, $this->local_config);
  }

  /**
   * {@inheritdoc}
   */
  public function read($name) {
    return array_key_exists($name, $this->local_config) ? $this->local_config[$name] : FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function readMultiple(array $names) {
    return array_intersect_key($this->local_config, array_fill_keys($names, TRUE));
  }

  /**
   * Writes configuration data to the storage.
   *
   * @param string $name
   *   The name of a configuration object to save.
   * @param array $data
   *   The configuration data to write.
   *
   * @return bool
   *   TRUE on success, FALSE in case of an error.
   */
  public function write($name, array $data) {
    return FALSE;
  }

  /**
   * Fill the object with data.  This can be done once to initialize the object,
   * after which it will throw a ConfigStorageException.
   * @param array $data
   *   The configuration data.
   * @return bool
   *   TRUE on success.
   * @throws ConfigStorageException
   */
  public function writeMultiple(array $data) {
    throw new ConfigStorageException("Immutable object.");
  }

  /**
   * Deletes a configuration object from the storage.
   *
   * @param string $name
   *   The name of a configuration object to delete.
   *
   * @return bool
   *   TRUE on success, FALSE otherwise.
   */
  public function delete($name) {
    return FALSE;
  }

  /**
   * Renames a configuration object in the storage.
   *
   * @param string $name
   *   The name of a configuration object to rename.
   * @param string $new_name
   *   The new name of a configuration object.
   *
   * @return bool
   *   TRUE on success, FALSE otherwise.
   */
  public function rename($name, $new_name) {
    return FALSE;
  }


  /**
   * Returns a timestamp indicating the last time a configuration was modified.
   *
   * @param string $name
   *   The name of a configuration object on which the time will be checked.
   *
   * @return int
   *   A timestamp indicating the last time the configuration was modified.
   */
  public function getModifiedTime($name) {
    return $this->created[$name];
  }

  /**
   * Encodes configuration data into the storage-specific format.
   *
   * @param array $data
   *   The configuration data to encode.
   *
   * @return string
   *   The encoded configuration data.
   *
   * This is a publicly accessible static method to allow for alternative
   * usages in data conversion scripts and also tests.
   */
  public function encode($data) {
    throw new ConfigStorageException("Immutable object.");
  }

  /**
   * Decodes configuration data from the storage-specific format.
   *
   * @param string $raw
   *   The raw configuration data string to decode.
   *
   * @return array
   *   The decoded configuration data as an associative array.
   *
   * This is a publicly accessible static method to allow for alternative
   * usages in data conversion scripts and also tests.
   */
  public function decode($raw) {
    throw new ConfigStorageException("Immutable object.");
  }

  /**
   * Gets configuration object names starting with a given prefix.
   *
   * Given the following configuration objects:
   * - node.type.post
   * - node.type.page
   *
   * Passing the prefix 'node.type.' will return an array containing the above
   * names.
   *
   * @param string $prefix
   *   (optional) The prefix to search for. If omitted, all configuration object
   *   names that exist are returned.
   *
   * @return array
   *   An array containing matching configuration object names.
   */
  public function listAll($prefix = '') {
    return array_keys(array_filter($this->local_config, function($k) { return (strpos($k, $prefix) === 0); }, ARRAY_FILTER_USE_KEY));
  }

  /**
   * Deletes configuration objects whose names start with a given prefix.
   *
   * Given the following configuration object names:
   * - node.type.post
   * - node.type.page
   *
   * Passing the prefix 'node.type.' will delete the above configuration
   * objects.
   *
   * @param string $prefix
   *   (optional) The prefix to search for. If omitted, all configuration
   *   objects that exist will be deleted.
   *
   * @return boolean
   *   TRUE on success, FALSE otherwise.
   */
  public function deleteAll($prefix = '') {
    throw new ConfigStorageException("Immutable object.");
  }

  /**
   * Import an archive of configuration files into the config storage managed
   * by this object.
   *
   * @param string $file_uri
   *   The URI of the tar archive file to import.
   *
   * @throws ConfigStorageException
   *
   * @return bool
   *   TRUE on success, FALSE otherwise.
   */
  public function importArchive($file_uri) {
    throw new ConfigStorageException("Immutable object.");
  }

  /**
   * Export an archive of configuration files from the config storage managed
   * by this object.
   *
   * @param string $file_uri
   *   The URI of the tar archive file to create.
   *
   * @throws ConfigStorageException
   *
   * @return bool
   *   TRUE on success, FALSE otherwise.
   */
  public function exportArchive($file_uri) {
    throw new ConfigStorageException("Immutable object.");
  }
}
