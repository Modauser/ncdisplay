/* Auto-generated config file usbh_config.h */
#ifndef USBH_CONFIG_H
#define USBH_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// ---- USB Host Stack Core Options ----

// <o> Control process buffer size <64-1024>
// <i> Control process buffer is shared by all default endpoint 0 transfers
// <i> The size should be max supported descriptor size plus setup packet size (8) plus VID and PID (4)
// <id> usbh_ctrl_buf_size
#ifndef CONF_USBH_CTRL_BUF_SIZE
#define CONF_USBH_CTRL_BUF_SIZE 96
#endif

// <o> Max power (in mA) <0-10000:100>
// <i> Max power can supply by root hub, in mA.
// <i> 0 - The power is not checked (unlimited).
// <id> usbh_power_max
#ifndef CONF_USBH_POWER_MAX
#define CONF_USBH_POWER_MAX 500
#endif

// VID and PID are logged in shared control buffer
#ifndef CONF_USBH_VENDOR_DEV_SP
#define CONF_USBH_VENDOR_DEV_SP 1
#endif

// Multiple devices support in the USB bus tree
#ifndef CONF_USBH_MULTI_DEV_SP
#define CONF_USBH_MULTI_DEV_SP 0
#endif

// Hub support in the USB bus tree
#ifndef CONF_USBH_HUB_SP
#define CONF_USBH_HUB_SP 0
#endif

// ---- USB Host Stack MSC Options ----

// Number of cached LUN
// If LUN is cached, switching between LUN is optimized by not sending test_unit_ready ... commands
// Set to 1 to minimize the memory usage
// msch_n_cached_lun
#ifndef CONF_MSCHF_N_CACHED_LUN
#define CONF_MSCHF_N_CACHED_LUN 1
#endif

// <o> Operation timeout in ms <0-3000:500>
// <i> Set to 0 to disable timeout
// <id> msch_timeout
#ifndef CONF_MSCHF_TIMEOUT
#define CONF_MSCHF_TIMEOUT 1000
#endif

// <q> No write operation support
// <i> Optimize by limiting access to read only
// <id> msch_read_only
#ifndef CONF_MSCHF_READ_ONLY
#define CONF_MSCHF_READ_ONLY 0
#endif

// <<< end of configuration section >>>

#endif // USBH_CONFIG_H
