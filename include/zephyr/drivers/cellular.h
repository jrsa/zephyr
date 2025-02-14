/*
 * Copyright (c) 2023 Bjarki Arge Andreasen
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file drivers/cellular.h
 * @brief Public cellular network API
 */

#ifndef ZEPHYR_INCLUDE_DRIVERS_CELLULAR_H_
#define ZEPHYR_INCLUDE_DRIVERS_CELLULAR_H_

/**
 * @brief Cellular interface
 * @defgroup cellular_interface Cellular Interface
 * @ingroup io_interfaces
 * @{
 */

#include <zephyr/types.h>
#include <zephyr/device.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Cellular access technologies */
enum cellular_access_technology {
	CELLULAR_ACCESS_TECHNOLOGY_GSM = 0,
	CELLULAR_ACCESS_TECHNOLOGY_GPRS,
	CELLULAR_ACCESS_TECHNOLOGY_UMTS,
	CELLULAR_ACCESS_TECHNOLOGY_EDGE,
	CELLULAR_ACCESS_TECHNOLOGY_LTE,
	CELLULAR_ACCESS_TECHNOLOGY_LTE_CAT_M1,
	CELLULAR_ACCESS_TECHNOLOGY_LTE_CAT_M2,
	CELLULAR_ACCESS_TECHNOLOGY_NB_IOT,
};

/** Cellular network structure */
struct cellular_network {
	/** Cellular access technology */
	enum cellular_access_technology technology;
	/**
	 * List of bands, as defined by the specified cellular access technology,
	 * to enables. All supported bands are enabled if none are provided.
	 */
	uint16_t *bands;
	/** Size of bands */
	uint16_t size;
};

enum cellular_signal_type {
	CELLULAR_SIGNAL_RSSI,
	CELLULAR_SIGNAL_RSRP,
	CELLULAR_SIGNAL_RSRQ,
};

/** Cellular modem info type */
enum cellular_modem_info_type {
	/** International Mobile Equipment Identity */
	CELLULAR_MODEM_INFO_IMEI,
	/** Modem model ID */
	CELLULAR_MODEM_INFO_MODEL_ID,
	/** Modem manufacturer */
	CELLULAR_MODEM_INFO_MANUFACTURER,
	/** Modem fw version */
	CELLULAR_MODEM_INFO_FW_VERSION,
	/** International Mobile Subscriber Identity */
	CELLULAR_MODEM_INFO_SIM_IMSI,
	/** Integrated Circuit Card Identification Number (SIM) */
	CELLULAR_MODEM_INFO_SIM_ICCID,
};

/**
 * @brief Configure cellular networks for the device
 *
 * @details Cellular network devices support at least one cellular access technology.
 * Each cellular access technology defines a set of bands, of which the cellular device
 * will support all or a subset of.
 *
 * The cellular device can only use one cellular network technology at a time. It must
 * exclusively use the cellular network configurations provided, and will prioritize
 * the cellular network configurations in the order they are provided in case there are
 * multiple (the first cellular network configuration has the highest priority).
 *
 * @param dev Cellular network device instance.
 * @param networks List of cellular network configurations to apply.
 * @param size Size of list of cellular network configurations.
 *
 * @retval 0 if successful.
 * @retval -EINVAL if any provided cellular network configuration is invalid or unsupported.
 * @retval -ENOTSUP if API is not supported by cellular network device.
 * @retval Negative errno-code otherwise.
 */
int cellular_configure_networks(const struct device *dev, const struct cellular_network *networks,
				uint8_t size);

/**
 * @brief Get supported cellular networks for the device
 *
 * @param dev Cellular network device instance
 * @param networks Pointer to list of supported cellular network configurations.
 * @param size Size of list of cellular network configurations.
 *
 * @retval 0 if successful.
 * @retval -ENOTSUP if API is not supported by cellular network device.
 * @retval Negative errno-code otherwise.
 */
int cellular_get_supported_networks(const struct device *dev,
				    const struct cellular_network **networks, uint8_t *size);

/**
 * @brief Get signal for the device
 *
 * @param dev Cellular network device instance
 * @param type Type of the signal information requested
 * @param value Signal strength destination (one of RSSI, RSRP, RSRQ)
 *
 * @retval 0 if successful.
 * @retval -ENOTSUP if API is not supported by cellular network device.
 * @retval -ENODATA if device is not in a state where signal can be polled
 * @retval Negative errno-code otherwise.
 */
int cellular_get_signal(const struct device *dev, const enum cellular_signal_type type,
			int16_t *value);

/**
 * @brief Get modem info for the device
 *
 * @param dev Cellular network device instance
 * @param type Type of the modem info requested
 * @param info Info string destination
 * @param size Info string size
 *
 * @retval 0 if successful.
 * @retval -ENOTSUP if API is not supported by cellular network device.
 * @retval -ENODATA if modem does not provide info requested
 * @retval Negative errno-code from chat module otherwise.
 */
int cellular_get_modem_info(const struct device *dev, const enum cellular_modem_info_type type,
			    char *info, size_t size);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_DRIVERS_CELLULAR_H_ */
