#include <linux/pci.h>

#define MY_VENDOR_ID 0xABCD
#define MY_DEVICE_ID_NET 0xEF01

struct pci_device_id network_driver_pci_table[] __devinitdata = {
	{
		{
			MY_VENDOR_ID,
			MY_DEVICE_ID_NET,
			PCI_ANY_ID,
			0, 0,
		}
	}, {0},
};

struct pci_driver network_pci_driver = {
	.name = "ntwrk",
	.probe = net_driver_probe,
	.remove = __devexit(net_driver_remove),
	.id_table = network_driver_pci_table,
};

static int __init network_driver_init(void) {
	pci_register_driver(&network_pci_driver);
	return 0;
}

static void __exit network_driver_exit (void) {
	pci_unregister_driver(&network_pci_driver);
}

module_init (network_driver_init);
module_exit (network_driver_exit);
MODULE_DEVICE_TABLE(pci, network_driver_pci_table);
