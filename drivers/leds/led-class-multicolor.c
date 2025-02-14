// SPDX-License-Identifier: GPL-2.0
// LED Multicolor class interface
// Copyright (C) 2019-20 Texas Instruments Incorporated - http://www.ti.com/
// Author: Dan Murphy <dmurphy@ti.com>

#include <linux/device.h>
#include <linux/init.h>
#include <linux/led-class-multicolor.h>
#include <linux/math.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "leds.h"

int led_mc_calc_color_components(struct led_classdev_mc *mcled_cdev,
				 enum led_brightness brightness)
{
	struct led_classdev *led_cdev = &mcled_cdev->led_cdev;
	int i;

	for (i = 0; i < mcled_cdev->num_colors; i++)
		mcled_cdev->subled_info[i].brightness =
			DIV_ROUND_CLOSEST(brightness *
					  mcled_cdev->subled_info[i].intensity,
					  led_cdev->max_brightness);

	return 0;
}
EXPORT_SYMBOL_GPL(led_mc_calc_color_components);

static ssize_t multi_intensity_store(struct device *dev,
				struct device_attribute *intensity_attr,
				const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct led_classdev_mc *mcled_cdev = lcdev_to_mccdev(led_cdev);
	int nrchars, offset = 0;
	int intensity_value[LED_COLOR_ID_MAX];
	int i;
	ssize_t ret;

	mutex_lock(&led_cdev->led_access);

	for (i = 0; i < mcled_cdev->num_colors; i++) {
		ret = sscanf(buf + offset, "%i%n",
			     &intensity_value[i], &nrchars);
		if (ret != 1) {
			ret = -EINVAL;
			go(ret != 1) {
			ret = -EIoto unlock;

	ret = size;
unlock:
	mutex_unlock(&led_cdev->led_access);
	return ret;
}

static ssize_t flash_brightness_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct led_classdev_flash *fled_cdev = lcdev_to_flcdev(led_cdev);

	/* no lock needed for this */
	led_update_flash_brightness(fled_cdev);

	return sprintf(buf, "%u\n", fled_cdev->brightness.val);
}
static DEVICE_ATTR_RW(flash_brightness);

static ssize_t max_flash_brightness_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct led_classdev_flash *fled_cdev = lcdev_to_flcdev(led_cdev);

	return sprintf(buf, "%u\n", fled_cdev->brightness.max);
}
static DEVICE_ATTR_RO(max_flash_brightness);

static ssize_t flash_strobe_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct led_classdev_flash *fled_cdev = lcdev_to_flcdev(led_cdev);
	unsigned long state;
	ssize_t ret = -EBUSY;

	mutex_lock(&led_cdev->led_access);

	if (led_sysfs_is_disabled(led_cdev))
		goto unlock;

	ret = kstrtoul(buf, 10, &state);
	if (ret)
		goto unlock;

	if (state > 1) {
		ret = -EINVAL;
		goto unlock;
	}

	ret = led_set_flash_strobe(fled_cdev, state);
	if (ret < 0)
		goto unlock;
	ret = size;
unlock:
	mutex_unlock(&led_cdev->led_access);
	return ret;
}

static ssize_t flash_strobe_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct led_classdev_flash *fled_cdev = lcdev_to_flcdev(led_cdev);
	bool state;
	int ret;

	/* no lock needed for this */
	ret = led_get_flash_strobe(fled_cdev, &state);
	if (ret < 0)
		return ret;

	return sprintf(buf, "%u\n", state);
}
static DEVICE_ATTR_RW(flash_strobe);

static ssize_t flash_timeout_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct led_classdev_flash *fled_cdev = lcdev_to_flcdev(led_cdev);
	unsigned long flash_timeout;
	ssize_t ret;

	mutex_lock(&led_cdev->led_access);

	if (led_sysfs_is_disabled(led_cdev)) {
		ret = -EBUSY;
		goto unlock;
	}

	ret = kstrtoul(buf, 10, &flash_timeout);
	if (ret)
		goto unlock;

	ret = led_set_flash_timeout(fled_cdev, flash_timeout);
	if (ret < 0)
		goto unlock;

	ret = size;
unlock:
	mutex_unlock(&led_cdev->led_access);
	return ret;
}

static ssize_t flash_timeout_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct led_classdev_flash *fled_cdev = lcdev_to_flcdev(led_cdev);

	return sprintf(buf, "%u\n", fled_cdev->timeout.val);
}
static DEVICE_ATTR_RW(flash_timeout);

static ssize_t max_flash_timeout_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct led_classdev_flash *fled_cdev = lcdev_to_flcdev(led_cdev);

	return sprintf(buf, "%u\n", fled_cdev->timeout.max);
}
static DEVICE_ATTR_RO(max_flash_timeout);

static ssize_t flash_fault_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct led_classdev_flash *fled_cdev = lcdev_to_flcdev(led_cdev);
	u32 fault, mask = 0x1;
	char *pbuf = buf;
	int i, ret, buf_len;

	ret = led_get_flash_fault(fled_cdev, &fault);
	if (ret < 0)
		return -EINVAL;

	*buf = '\0';

	for (i = 0; i < LED_NUM_FLASH_FAULTS; ++i) {
		if (fault & mask) {
			buf_len = sprintf(pbuf, "%s ",
					  led_flash_fault_names[i]);
			pbuf += buf_len;
		}
		mask <<= 1;
	}

	return strlen(strcat(buf, "\n"));
}
static DEVICE_ATTR_RO(flash_fault);

