#ifndef MSP_CMD_H
#define MSP_CMD_H

#include <msp/msp.h>

#define MSP_CMD_TIMEOUT (struct timeval) { 1, 0 }

int msp_acc_calibration(struct msp *msp);

int msp_attitude(struct msp *msp, struct msp_attitude *att, size_t *len);

int msp_altitude(struct msp *msp, struct msp_altitude *att, size_t *len);

int msp_analog(struct msp *msp, struct msp_analog *bat, size_t *len);

int msp_box(struct msp *msp, uint16_t *box, int *cnt);

int msp_boxids(struct msp *msp, uint8_t *boxids, size_t *len);

int msp_boxnames(struct msp *msp, char *names, size_t *len);

int msp_eeprom_write(struct msp *msp);

int msp_ident(struct msp *msp, struct msp_ident *ident, size_t *len);

int msp_mag_calibration(struct msp *msp);

int msp_motor(struct msp *msp, struct msp_motor *motor, size_t *len);

int msp_motor_pins(struct msp *msp, struct msp_motor_pins *pins, size_t *len);

int msp_raw_imu(struct msp *msp, struct msp_raw_imu *imu);

int msp_rc(struct msp *msp, struct msp_raw_rc *rrc, size_t *len);

int msp_reset_conf(struct msp *msp);

int msp_servo(struct msp *msp, struct msp_servo *servo, size_t *len);

int msp_set_box(struct msp *msp, uint16_t *items, int cnt);

int msp_set_raw_rc(struct msp *msp, struct msp_raw_rc *rrc);

int msp_status(struct msp *msp, struct msp_status *st, size_t *len);

#endif

/*
 * Local variables:
 * mode: C
 * c-file-style: "Linux"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
