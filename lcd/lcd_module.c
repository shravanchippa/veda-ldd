#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <asm/io.h>
#include <linux/ioport.h>

#define LCDCON1 0X4d000000  
#define LCDCON2 0X4D000004   
#define LCDCON3 0X4D000008 
#define LCDCON4 0X4D00000C 
#define LCDCON5 0X4D000010 
#define LCDSADDR1 0X4D000014 
#define LCDSADDR2 0X4D000018  
#define LCDSADDR3 0X4D00001C  
#define REDLUT 0X4D000020  
#define GREENLUT 0X4D000024  
#define BLUELUT 0X4D000028  
#define DITHMODE 0X4D00004C 
#define LCDINTPND 0X4D000054  
#define LCDSRCPND 0X4D000058 
 unsigned int stn=0;
unsigned int tft=0;
unsigned int mode;
 void lcd1_config(void *lcdcon1)
{
	unsigned int clk,pnrmode,Lcdcon1,bpp,envid;
	Lcdcon1=ioread32(lcdcon1);
	
	clk=(Lcdcon1>>8)&(0x3ff);
	pr_info("clk value is %x\n",clk);

	mode=(Lcdcon1>>7)&(0x01);
	pr_info("mode value is %x\n",mode);
	if(mode==0)
		pr_info("Each Frame \n");
	else
		pr_info("The rate defined by the MVAL \n");
	
	pnrmode=(Lcdcon1>>5)&(0x03);
	pr_info("Display mode:\n");
	switch(pnrmode){
		case 0:
			pr_info("4-bit dual scan display mode (STN)\n");
			break;
		case 1:
			pr_info("4-bit single scan display mode (STN)\n");
			break;
		case 2:
			pr_info("8-bit single scan display mode (STN)\n");
			break;
		case 3:
			pr_info("11 = TFT LCD panel\n");
			break;
	}

	bpp=(Lcdcon1>>1)&(0x0f);
	switch(bpp){
                case 0:
                        pr_info("1 bpp for STN, Monochrome mode\n");
                        break;
                case 1:
                        pr_info("2 bpp for STN, 4-level gray mode\n");
                        break;
                case 2:
                        pr_info("4 bpp for STN, 16-level gray mode\n");
                        break;
                case 3:
                        pr_info("8 bpp for STN, color mode (256 color)\n");
                        break;
		case 4:
                        pr_info("packed 12 bpp for STN, color mode (4096 color)\n");
                        break;
                case 5:
                        pr_info("unpacked 12 bpp for STN, color mode (4096 color)\n");
                        break;
                case 6:
                        pr_info("16 bpp for STN, color mode (4096 color)\n");
                        break;
                case 8:
                        pr_info("1 bpp for TFT\n");
                        break;
		case 9:
                        pr_info("2 bpp for TFT\n");
                        break;
                case 10:
                        pr_info("4 bpp for TFT\n");
                        break;
                case 11:
                        pr_info("8 bpp for TFT\n");
                        break;
                case 12:
                        pr_info("16 bpp for TFT\n");
                        break;
		case 13:
                        pr_info("24 bpp for TFT\n");
                        break;

        }
	
	envid=Lcdcon1&(0x01);
	if(envid==0)
		pr_info(" the video output and the LCD control signal disabled\n");
	else 
		pr_info(" the video output and the LCD control signal enabled\n");
		

}

void lcd2_config(void *lcdcon2)
{
	unsigned int lineval,Lcdcon2,vbpd,vfpd,vspw;
	Lcdcon2=ioread32(lcdcon2);
	lineval=(Lcdcon2>>14)&(0x03ff);
	pr_info("lineval is %x\n this represent vertical size of lcd panel\n",lineval);
	
	vbpd=(Lcdcon2>>24)&(0xff);
	if(vbpd==0){
			stn=1;
		pr_info("lcd is used as STN LCD\n");
		}
	else{
		tft=1;
		pr_info("number of inactive lines at the start of a frame :%x\n",vbpd);
	}

	vfpd=(Lcdcon2>>6)&(0xff);
	if(vbpd==0)
		pr_info("\n");
	else
	pr_info("number of inactive lines at the end of a frame:%x\n",vfpd);
	
	vspw=Lcdcon2&(0x3f);
	if(vspw==0)
		pr_info("\n");
	else
		pr_info(" number of inactive lines:%x\n",vspw);
	
}

void lcd3_config(void *lcdcon3)
{
	unsigned int hozval,Lcdcon3,hbpd,wdly,hfpd,lineblank;

	Lcdcon3=ioread32(lcdcon3);
	if(stn==1)
	{
		 wdly=(Lcdcon3>>19)&(0x7f);
			unsigned int sh=wdly&(0x03);
		pr_info("delay between VLINE and VCLK by counting the number of the HCLK :%x\n",wdly&(0x03));
		switch(sh){
			case 0:
				pr_info("16 HCLK\n");
				break;
			case 1:
				pr_info("32 HCLK\n");
				break;
			case 2:
				pr_info("48 HCLK\n");
				break;
			case 3:
				pr_info("64 HCLK\n");
				break;
	}
}
	else if(tft==1){
		hbpd=(Lcdcon3>>19)&(0x7f);
		
		pr_info("number of VCLK periods between the falling edge of HSYNC and the start of active data:%x\n",hbpd);
	}

	hozval=(Lcdcon3>>8)&(0x7ff);
	
	pr_info("hozval is %x\n These bits determine the horizontal size of LCD panel\n",hozval);

	if(stn==1){
		lineblank=Lcdcon3&(0xff);
	pr_info("lineblank is %x\n These bits indicate the blank time in one horizontal line\
			duration time. These bits adjust the rate of the VLINE finely\n",lineblank);
	}
	else if(tft==1){
			hfpd=Lcdcon3&(0xff);
		pr_info(" number of VCLK periods between\
					the end of active data and the rising edge of HSYNC :%x\n",hfpd);
	
}

}


void lcd4_config(void *lcdcon4)
{
	unsigned int mval,hspw,wlh,Lcdcon4;

	Lcdcon4=ioread32(lcdcon4);
	if(mode==0)
		pr_info("\n");
	else{
	mval =(Lcdcon4>>8)&(0xff);
	pr_info("the rate at which the VM signal will toggle :%x\n",mval);
	}
	        
	if(stn==1){
                wlh=Lcdcon4&(0xff);
                unsigned int sh=wlh&(0x03);
                pr_info("VLINE pulse's high level width is:%x\n",wlh&(0x03));
                switch(sh){
                        case 0:
                                pr_info("16 HCLK\n");
                                break;
                        case 1:
                                pr_info("32 HCLK\n");
                                break;
                        case 2:
                                pr_info("48 HCLK\n");
                                break;
                        case 3:
                                pr_info("64 HCLK\n");
                                break;
        }

        }
        else if(tft==1){
                        hspw=Lcdcon4&(0xff);
                pr_info("Horizontal sync pulse width  :%x\n",hspw);
}
}

void lcd5_config(void *lcdcon5)
{
	unsigned int Lcdcon5,vstatus,hstatus,bpp24bl,frm565,invvclk,invvline,invvframe,invvd;
	unsigned int invvden,invpwren,invlend,pwren,enlend,bswp,hwswp;
	Lcdcon5=ioread32(lcdcon5);

	vstatus=(Lcdcon5>>15)&(0x03);
	pr_info("vertical status :\n");
	
	switch(vstatus){
			
			case 0:
                                pr_info("VSYNC \n");
                                break;
                        case 1:
                                pr_info("BACK Porch \n");
                                break;
                        case 2:
                                pr_info(" ACTIVE \n");
                                break;
                        case 3:
                                pr_info("FRONT Porch \n");
                                break;
}

	hstatus=(Lcdcon5>>13)&(0x03);
	pr_info("horizontal status :\n");

        switch(hstatus){
                
                        case 0:
                                pr_info("HSYNC \n");
                                break;
                        case 1:
                                pr_info("BACK Porch \n");
                                break;
                        case 2:
                                pr_info(" ACTIVE \n");
                                break;
                        case 3:
                                pr_info("FRONT Porch \n");
                                break;
}
	
	frm565=(Lcdcon5>>11)&(0x01);
	pr_info("format of 16 bpp output video data:\n");
	if(frm565==0)
		pr_info("5:5:5:1 Format \n");
		else
		pr_info("5:6:5 Format \n");
		
	invvclk=(Lcdcon5>>10)&(0x01);
	if(invvclk==0)
		pr_info("The video data is fetched at VCLK falling edge \n");
		else
		pr_info("The video data is fetched at VCLK rising edge \n");

	invvline=(Lcdcon5>>9)&(0x01);
	if(invvline==0)
                pr_info("VLINE/HSYNC pulse polarity is normal \n");
                else
                pr_info("VLINE/HSYNC pulse polarity is inverted \n");
	invvframe=(Lcdcon5>>9)&(0x01);
	if(invvline==0)
                pr_info("VFRAME/VSYNC pulse polarity is normal\n");
                else
                pr_info("VFRAME/VSYNC pulse polarity is inverted \n");
	
	invvd=(Lcdcon5>>7)&(0x01);
	 if(invvd==0)
                pr_info("VD (video data) pulse polarity is normal\n");
                else
                pr_info("VD (video data) pulse polarity is inverted \n");

	invvden=(Lcdcon5>>6)&(0x01);
         if(invvd==0)
                pr_info("VDEN signal polarity normal\n");
                else
                pr_info("VDEN signal polarity inverted \n");

	 invpwren=(Lcdcon5>>5)&(0x01);
         if(invvd==0)
                pr_info("PWREN signal polarity  normal\n");
                else
                pr_info("PWREN signal polarity inverted \n");

	 invlend=(Lcdcon5>>4)&(0x01);
         if(invvd==0)
                pr_info("LEND signal polarity  normal\n");
                else
                pr_info("LEND signal polarity  inverted \n");

	 pwren=(Lcdcon5>>3)&(0x01);
         if(invvd==0)
                pr_info(" LCD_PWREN output signal disabled\n");
                else
                pr_info("  LCD_PWREN output signal enabled \n");

	 enlend=(Lcdcon5>>2)&(0x01);
         if(invvd==0)
                pr_info("LEND output signal disabled\n");
                else
                pr_info("LEND output signal enabled \n");

	 bswp=(Lcdcon5>>1)&(0x01);
         if(invvd==0)
                pr_info("Byte swap control bit disabled\n");
                else
                pr_info("Byte swap control bit enabled \n");

	 hwswp=(Lcdcon5>>0)&(0x01);
         if(invvd==0)
                pr_info("Half-Word swap control bit disabled\n");
                else
                pr_info("Half-Word swap control bit enabled \n");

}


int init_module(void)
{
	pr_info("module registered\n");
	/*first we need to remap the physical address to logical address */ 
			
	void *log_addr=ioremap(LCDCON1,sizeof(int)*3);

	void *lcdcon1,*lcdcon2,*lcdcon3,*lcdcon4,*lcdcon5,*redlut,*greenlut,*bluelut;
	lcdcon1=log_addr;
	lcdcon2=log_addr+0x04;
	lcdcon3=log_addr+0x08;
	lcdcon4=log_addr+0x0c;
	lcdcon5=log_addr+0x10;
	redlut=log_addr+0x20;
	greenlut=log_addr+0x24;
	bluelut=log_addr+0x28;
	
	lcd1_config(lcdcon1);
	lcd2_config(lcdcon2);
	lcd3_config(lcdcon3);
	lcd4_config(lcdcon4);
	lcd5_config(lcdcon5);
	
return 0;	


}


void cleanup_module(void)
{

pr_info("module removed\n");

}

MODULE_LICENSE("GPL");
