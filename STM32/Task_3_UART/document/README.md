1.	Tx（发送数据）接Rx（接收数据）
	Rx接Tx
2.	要将USB时钟调整为48MHZ
3.     使用CDC_Transmit_FS((uint8_t*)自定名,strlen(自定名))向电脑发送信息
4.	使用CDC_Transmit_FS(BUF,*Len)进行发送接收

https://github.com/user-attachments/assets/b500ec64-d744-409c-bf5c-d06ddc21e8c5
https://github.com/user-attachments/assets/ae44a952-58d7-474e-9b49-1895d81f2d02

