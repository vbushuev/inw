#include "funcs.h"
extern int gRegisters[128];
byte in[COMPORT_BUFFER_LENGTH];
byte out[COMPORT_BUFFER_LENGTH];//=":010311112222333344445555";
int readModbusRTU(){
	int ret=0, i=0,inlen,outlen;
	unsigned int crc;
	sModbusPack mb;
	// clean buffers
	memset(in,0,COMPORT_BUFFER_LENGTH);
	memset(out,0,COMPORT_BUFFER_LENGTH);
	// receive data
	ret = Receive_Data_Length(in,8,1000);
	//ret = Receive_Data(in,0xa,TOTAL_TIMEOUT);
    if(ret<=0) return 0;
	//parse data
	mb.addr = in[0];
	mb.func = in[1];
	mb.reg	= in[2]*256 + in[3];
	mb.data = in[4]*256 + in[5];
	mb.lrc	= in[6]*256 + in[7];
	/*ledn(0xa,mb.addr);DelayMs(600);
	ledn(0xf,mb.func);DelayMs(600);
	ledn(16,mb.reg);DelayMs(600);
	ledn(0xd,mb.data);DelayMs(600);*/
	switch(mb.func){
		case 0x01:
		case 0x02:{// read coil registers
			int tail=mb.data%8;
			out[0] = mb.addr;
			out[1] = mb.func;
			out[2] = (mb.data/8) + ((tail>0)?1:0);
			mb.response_size=(int)out[2];
			for(i=0;i<mb.response_size;i++){
				byte bt[2];
				bt[1]=gRegisters[mb.reg+i]/256;
				bt[0]=gRegisters[mb.reg+i]%256;

				memcpy(out+3+(2*i),bt,2);
            }
			if(tail>0){
				out[mb.response_size+2] <<= (8-tail);
				out[mb.response_size+2] >>= (8-tail);
			}
			crc=CRC16(out,mb.response_size+3);
			//ledn(0xc,crc);
		    mb.lrc=crc;
			out[mb.response_size+3] = crc%256;
			out[mb.response_size+4] = crc/256;
			ret = mb.response_size+5;
		}break;
		case 0x03:
        case 0x04:{ // read coil registers
			out[0] = mb.addr;
			out[1] = mb.func;
			out[2] = 2*mb.data;
            for(i=0;i<mb.data;i++){
                out[3+(2*i)] = gRegisters[mb.reg+i]/256;
				out[3+(2*i)+1] =gRegisters[mb.reg+i]%256;
            }
			mb.response_size=2*mb.data;
			crc=CRC16(out,mb.response_size+3);
			//ledn(0xc,crc);
		    mb.lrc=crc;
			out[mb.response_size+3] = crc%256;
			out[mb.response_size+4] = crc/256;
			ret = mb.response_size+5;
		}break;
        case 0x05:
		case 0x06:{ // write single register
			memcpy(out,in,ret);
			gRegisters[mb.reg] = mb.data;
            setRegisters();
		}break;
	}
	ToComBufn(COMPORT,out,ret);
	DelayMs(4);
    return ret;
}
/*
sModbusPack parse(byte* in){
	sModbusPack ret;
    ret.colon=':';
	ret.addr=16*ascii_to_hex(in[1]) + ascii_to_hex(in[2]);
	ret.func=16*ascii_to_hex(in[3]) + ascii_to_hex(in[4]);
	ret.reg	=4096*ascii_to_hex(in[5]) + 256*ascii_to_hex(in[6]) + 16*ascii_to_hex(in[7]) + ascii_to_hex(in[8]);
	ret.data=4096*ascii_to_hex(in[9]) + 256*ascii_to_hex(in[10]) + 16*ascii_to_hex(in[11]) + ascii_to_hex(in[12]);
	ret.lrc	=16*ascii_to_hex(in[13]) + ascii_to_hex(in[14]);
	return ret;
}
int serialize(psModbusPack pmb,byte *out){
	int i=0;
    byte ret[COMPORT_BUFFER_LENGTH];
    byte reta[COMPORT_BUFFER_LENGTH];
    byte l;
	memset(ret,0,COMPORT_BUFFER_LENGTH);
	memset(reta,0,COMPORT_BUFFER_LENGTH);
	ret[i++] = pmb->colon;
	ret[i++] = pmb->addr/16;
	ret[i++] = pmb->addr%16;
	ret[i++] = pmb->func/16;
	ret[i++] = pmb->func%16;
	ret[i++] = pmb->response_size/16;
	ret[i++] = pmb->response_size%16;
    memcpy(ret+i,pmb->response,pmb->response_size);
    i+=pmb->response_size;
    //i+=str_hex_to_ascii(test,10,ret+i);
    //pmb->response_size=10;
    l=lrc(ret+1,i-1);
    pmb->lrc=l;
    i=str_hex_to_ascii(ret+7,i-7,reta+7);
    i+=7;
    reta[0]=':';
    reta[1]=hex_to_ascii[ret[1]];
    reta[2]=hex_to_ascii[ret[2]];
    reta[3]=hex_to_ascii[ret[3]];
    reta[4]=hex_to_ascii[ret[4]];
    reta[5]=hex_to_ascii[ret[5]];
    reta[6]=hex_to_ascii[ret[6]];
    reta[i++] = hex_to_ascii[l/16];
	reta[i++] = hex_to_ascii[l%16];
	reta[i++] = '\r';
	reta[i++] = '\n';
    memset(out,0,COMPORT_BUFFER_LENGTH);
    memcpy(out,reta,i);
    return i;
}
int analyzeModBusASCII(psModbusPack pmb,byte* outbuf){
	int ret =0, sl = 0, i=0;
    byte o[COMPORT_BUFFER_LENGTH];
    byte data[COMPORT_BUFFER_LENGTH];
    int registers[128];
    ret = getRegisters(registers);
    if(ret!=0)exception(ret);
    memset(data,0,COMPORT_BUFFER_LENGTH);
    memset(o,0,COMPORT_BUFFER_LENGTH);
	switch(pmb->func){
		case 0x03:
        case 0x04:{ // read coil registers
            //EE_MultiRead(EEPROM_RUNTIME,pmb->reg,2*pmb->data,(byte*)(data));
            for(i=0;i<pmb->data;i++){
                byte bt[2];
                to_bytes(bt,gRegisters[pmb->reg+i]);
                memcpy(pmb->response+(2*i),bt,2);
            }
			pmb->response_size=2*pmb->data;
            sl=serialize(pmb,o);
            memset(outbuf,0,COMPORT_BUFFER_LENGTH);
            memcpy(outbuf,o,sl);
            ret = sl;
		}break;
        case 0x05:
		case 0x06:{ // write single register
            gRegisters[pmb->reg] = pmb->data;
            ret = setRegisters(gRegisters);
            if(ret!=0)exception(ret);
		}break;
	}
    //Show5DigitLed(1,10); leds(pmb->addr);//addr
	//Show5DigitLed(1,15); leds(pmb->func);
	//Show5DigitLed(1,13); leds(pmb->reg);
	//Show5DigitLed(1,17); leds(pmb->data);
	return ret;
}
int analyzeModBus(psModbusPack pmb,byte* outbuf){
	int ret =0, sl = 0, i=0;
    byte o[COMPORT_BUFFER_LENGTH];
    byte data[COMPORT_BUFFER_LENGTH];
    int registers[128];
    ret = getRegisters(registers);
    if(ret!=0)exception(ret);
    memset(data,0,COMPORT_BUFFER_LENGTH);
    memset(o,0,COMPORT_BUFFER_LENGTH);
	switch(pmb->func){
		case 0x01:
		case 0x02:{// read coil registers
			int rs = pmb->reg/8;
			int tail = pmb->data%8;
			rs+=pmb->data/8;
			rs+=(tail>0)?1:0;
			for(i=0;i<rs;i++){
                byte bt[2];
                to_bytes(bt,gRegisters[pmb->reg+i]);
                memcpy(pmb->response+(2*i),bt,2);
            }
			pmb->response_size=2*pmb->data;
            sl=serialize(pmb,o);
            memset(outbuf,0,COMPORT_BUFFER_LENGTH);
            memcpy(outbuf,o,sl);
            ret = sl;
		}break;
		case 0x03:
        case 0x04:{ // read hold registers
            for(i=0;i<pmb->data;i++){
                byte bt[2];
                to_bytes(bt,gRegisters[pmb->reg+i]);
                memcpy(pmb->response+(2*i),bt,2);
            }
			pmb->response_size=2*pmb->data;
            sl=serialize(pmb,o);
            memset(outbuf,0,COMPORT_BUFFER_LENGTH);
            memcpy(outbuf,o,sl);
            ret = sl;
		}break;
        case 0x05:
		case 0x06:{ // write single register
            gRegisters[pmb->reg] = pmb->data;
            ret = setRegisters(gRegisters);
            if(ret!=0)exception(ret);
		}break;
	}
    //Show5DigitLed(1,10); leds(pmb->addr);//addr
	//Show5DigitLed(1,15); leds(pmb->func);
	//Show5DigitLed(1,13); leds(pmb->reg);
	//Show5DigitLed(1,17); leds(pmb->data);
	return ret;
}
int readModbus(){
	int ret=0, i=0,inlen;
	byte inbuf[COMPORT_BUFFER_LENGTH],outbuf[COMPORT_BUFFER_LENGTH],response[COMPORT_BUFFER_LENGTH];//=":010311112222333344445555";
	byte *pbuf;
	sModbusPack mb;
	ret = Receive_Data(inbuf,0xa,TOTAL_TIMEOUT);
    if(ret<=0) return 0;
	pbuf=inbuf;
	while(*pbuf!=':'){
        pbuf++;
		i++;
		if(i>=ret)return 0;
    }
	ret-=i;
    mb=parse(pbuf);
	memset(outbuf,0,COMPORT_BUFFER_LENGTH);
    memcpy(outbuf,inbuf,ret);
    outbuf[ret]='\r';
    outbuf[ret+1]='\n';
    analyzeModBusASCII(&mb,outbuf);
	//serialize(&mb,outbuf);
	ToComStr(COMPORT,outbuf);
	//ToComStr(COMPORT,":010311112222333344445555F2\r\n");
    //log2EEPROM(inbuf,outbuf,mb.response_size+8);
    //logCOM(pbuf,inlen,outbuf,outlen);
    //Show5DigitLed(1,17);
    //leds(outbuf[2]*256+outbuf[4]);
    //Show5DigitLed(1,14); leds(mb.response_size+8);

	//Show5DigitLed(1,12);
	//leds(mb.lrc);

    return ret;
}
*/
