Memory Configuration Driver (config_memory)
===========================================

This driver contains the Memory-based configuration driver for Silkscreen CMS.

To enable this driver, place it in the `drivers` directory in the root of the
Silkscreen CMS site or in the sites/[yoursite]/drivers directory. Memory-based
configuration pools can be specified with `mem:<variable_name>` where
`variable_name` is a key in the $settings['config'] array.

For example:

```php
// Define a memory-based config object reading from mem_config.
$config_directories['active'] = 'mem:mem_config';

// Create some memory configurations.
$settings['config']['mem_config'] = array(
  'system.core' => array(
    'site_name' => 'My Site Name',
  ),
);
```

This will define a memory configuration object with only `site_name` in
`system.core`.

This configuration tool is most often used with the
[Layered](https://github.com/silkscreencms-contrib/config_layered) configuration
driver.

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.

Maintainers
-----------

- John Franklin (https://github.com/jlfranklin/)
