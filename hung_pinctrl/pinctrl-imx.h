#ifndef _PINCTRL_IMX_H_
#define _PINCTRL_IMX_H_

struct platform_device;

/*
* Function: describe a pin
* @pin: the pin_id of pin
* @mux_mode: select mode for this pin
* @input_reg: register select input
* @input_val: value of register select input
* @config: config for this pin
*/
struct imx_pin {
	unsigned int pin;
	unsigned int mux_mode;
	u16 input_reg;
	unsigned int input_val;
	unsigned long config;
};

/*
* Function: describe a group pin
* @name: name of group
* @npins: the number of pin in group
* @pin_ids: the array of pin_id of pin in group
* @pins: array of pins
*/
struct imx_pin_group {
	const char *name;
	unsigned npins;
	unsigned int *pin_ids;
	struct imx_pin *pins;
};

/*
* Function: describe a function
* @name: name of function
* @groups: corresponding pin groups
* @num_groups: the number of groups
*/
struct imx_pmx_func {
	const char *name;
	const char **groups;
	unsigned num_groups;
};

/*
* Function: describe a pin reg map
* @mux_reg: address offset of mux
* @conf_reg: address offset of config
*/
struct imx_pin_reg {
	u16 mux_reg;
	u16 conf_reg;
};


struct imx_pinctrl_soc_info {
	struct device *dev;
	const struct pinctrl_pin_desc *pins;
	unsigned int npins;
	struct imx_pin_reg *pin_regs;
	struct imx_pin_group *groups;
	unsigned int ngroups;
	struct imx_pmx_func *functions;
	unsigned int nfunctions;
	unsigned int flags;
};

#define ZERO_OFFSET_VALID	0x1
#define SHARE_MUX_CONF_REG	0x2

#define NO_MUX		0x0
#define NO_PAD		0x0

#define IMX_PINCTRL_PIN(pin) PINCTRL_PIN(pin, #pin)

#define PAD_CTL_MASK(len)	((1 << len) - 1)
#define IMX_MUX_MASK	0x7
#define IOMUXC_CONFIG_SION	(0x1 << 4)

int imx_pinctrl_probe(struct platform_device *pdev,
			struct imx_pinctrl_soc_info *info);
int imx_pinctrl_remove(struct platform_device *pdev);
#endif /* __DRIVERS_PINCTRL_IMX_H */