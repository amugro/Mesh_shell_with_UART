/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/sys/printk.h>
#include <stdlib.h>
#include <zephyr/kernel.h>

#include <zephyr/shell/shell.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/mesh.h>
#include <zephyr/bluetooth/mesh/shell.h>

#include "model_handler.h"

//additional includes
#include <zephyr/bluetooth/mesh/proxy.h>
#include <bluetooth/mesh/models.h>
#include <dk_buttons_and_leds.h>

static struct bt_mesh_cfg_cli cfg_cli;


#if defined(CONFIG_BT_MESH_DFD_SRV)
static struct bt_mesh_dfd_srv dfd_srv;
#endif

#if defined(CONFIG_BT_MESH_SAR_CFG_CLI)
static struct bt_mesh_sar_cfg_cli sar_cfg_cli;
#endif

#if defined(CONFIG_BT_MESH_PRIV_BEACON_CLI)
static struct bt_mesh_priv_beacon_cli priv_beacon_cli;
#endif

#if defined(CONFIG_BT_MESH_SOL_PDU_RPL_CLI)
static struct bt_mesh_sol_pdu_rpl_cli srpl_cli;
#endif


#if defined(CONFIG_BT_MESH_OD_PRIV_PROXY_CLI)
static struct bt_mesh_od_priv_proxy_cli od_priv_proxy_cli;
#endif

#if defined(CONFIG_BT_MESH_LARGE_COMP_DATA_CLI)
struct bt_mesh_large_comp_data_cli large_comp_data_cli;
#endif

#if defined(CONFIG_BT_MESH_BRG_CFG_CLI)
static struct bt_mesh_brg_cfg_cli brg_cfg_cli;
#endif



static void bt_ready(int err)
{
	if (err && err != -EALREADY) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");


	err = bt_mesh_init(&bt_mesh_shell_prov, model_handler_init());
	if (err) {
		printk("Initializing mesh failed (err %d)\n", err);
		return;
	}

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	printk("Mesh initialized\n");

	if (bt_mesh_is_provisioned()) {
		printk("Mesh network restored from flash\n");
	} else {
		printk("Use \"prov pb-adv on\" or \"prov pb-gatt on\" to "
			    "enable advertising\n");
	}
}

int main(void)
{
	int err;

	printk("Initializing...\n");

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(bt_ready);
	if (err && err != -EALREADY) {
		printk("Bluetooth init failed (err %d)\n", err);
	}

	printk("Press the <Tab> button for supported commands.\n");
	printk("Before any Mesh commands you must run \"mesh init\"\n");
	return 0;
}
