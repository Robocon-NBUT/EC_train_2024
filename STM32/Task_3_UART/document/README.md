1.	Tx（发送数据）接Rx（接收数据）
	Rx接Tx
2.	要将USB时钟调整为48MHZ
3.     使用CDC_Transmit_FS((uint8_t*)自定名,strlen(自定名))向电脑发送信息
4.	使用CDC_Transmit_FS(BUF,*Len)进行发送接收