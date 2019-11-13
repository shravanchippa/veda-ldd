#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/io.h>


# define KBOARD_BASE_PORT 0x60
# define KBOARD_DATA_PORT KBOARD_BASE_PORT
# define KBOARD_STATUS_REG KBOARD_BASE_PORT + 0x4 /* for reading status */
# define KBOARD_CTL_REG KBOARD_BASE_PORT + 0x4 /* for writing commands */

#define SHARED_IRQ 1 
static int irq = SHARED_IRQ, my_dev_id;
module_param (irq, int, S_IRUGO);

static unsigned char key_codes[10]={82,79,80,81,75,76,77,71,72,73};
/* scancodes of 0,1,2,3,4,5,6,7,8,9 on numeric pad */
static unsigned char key_ascii_codes[10]={49,50,51,52,53,54,55,56,57,58};


unsigned char key_code(unsigned char scancode){
        int i;
        for(i=0;i<10;i++){
                if(key_codes[i] == scancode )
                        return i;
        }
        return 255; //unknown key indicator
}


static irqreturn_t my_interrupt (int irq, void *dev_id)
{
	unsigned char scancode, up_code, key;
        scancode = inb(KBOARD_DATA_PORT);
	up_code= key_code(scancode);
	key =key_ascii_codes[up_code];
	printk(" key hit %u\n", key);
	return IRQ_NONE;
}

static int __init my_init (void)
{
   /* 
     arg 2: function pointer to assign interrupt context isr
     arg 3: function pointer to assign thread context isr
   */
    if (request_threaded_irq(irq, NULL, my_interrupt, IRQF_SHARED, "my_interrupt", &my_dev_id))
        return -1;
    printk (KERN_INFO "Successfully loading ISR handler\n");
    return 0;
}

static void __exit my_exit (void)
{
    synchronize_irq (irq);
    free_irq (irq, &my_dev_id);
    printk (KERN_INFO "Successfully unloading \n");
}

module_init (my_init);
module_exit (my_exit);

MODULE_AUTHOR ("Team veda");
MODULE_DESCRIPTION ("Interrupt Handler Test");
MODULE_LICENSE ("GPL");
