/*
 * Copyright (c) 2012, 2013 Samsung Electronics Co., Ltd.
 * Copyright (c) 2019 Collabora Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _MTP_USB_DRIVER_H_
#define _MTP_USB_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mtp_config.h"
#include "mtp_datatype.h"
#include "mtp_msgq.h"

/* End of driver related defines */

typedef struct mtp_max_pkt_size {
	mtp_uint32 tx;
	mtp_uint32 rx;
} mtp_max_pkt_size_t;

/* Transport driver interface */
typedef struct mtp_usb_driver {
	mtp_bool (*transport_init_usb_device)(void);
	void (*transport_deinit_usb_device)(void);
	void *(*transport_thread_usb_write)(void *arg);
	void *(*transport_thread_usb_read)(void *arg);
	void *(*transport_thread_usb_control)(void *arg);
	mtp_int32 (*transport_mq_init)(msgq_id_t *rx_ipc, msgq_id_t *tx_ipc);
	mtp_bool (*transport_mq_deinit)(msgq_id_t *rx_ipc, msgq_id_t *tx_ipc);
	mtp_uint32 (*transport_get_usb_packet_len)(void);
	mtp_uint32 (*get_tx_pkt_size)(void);
	mtp_uint32 (*get_rx_pkt_size)(void);
} mtp_usb_driver_t;

extern const mtp_usb_driver_t mtp_usb_driver_ffs;

/* Maximum repeat count for USB error recovery */
#define MTP_USB_ERROR_MAX_RETRY		5

mtp_bool _transport_init_usb_device(void);
void _transport_deinit_usb_device(void);
void *_transport_thread_usb_write(void *arg);
void *_transport_thread_usb_read(void *arg);
void *_transport_thread_usb_control(void *arg);
mtp_int32 _transport_mq_init(msgq_id_t *rx_mqid, msgq_id_t *tx_mqid);
mtp_bool _transport_mq_deinit(msgq_id_t *rx_mqid, msgq_id_t *tx_mqid);
mtp_uint32 _transport_get_usb_packet_len(void);
mtp_uint32 _get_tx_pkt_size(void);
mtp_uint32 _get_rx_pkt_size(void);

#ifdef __cplusplus
}
#endif

#endif /* _MTP_USB_DRIVER_H_ */
