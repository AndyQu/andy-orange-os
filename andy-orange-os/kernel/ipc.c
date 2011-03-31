/*This file is added in chapter8/a */
#include"const.h"
#include"type.h"
#include"protect_type.h"
#include"global.h"
#include"proto.h"
#include"assert.h"
#include"proc.h"

static int __DEBUG_Ipc=0;
static int __id=3;
/*****************************************************************************
 *                                msg_send
 *****************************************************************************/
/**
 * <Ring 0> Send a message to the dest proc. If dest is blocked waiting for
 * the message, copy the message to it and unblock dest. Otherwise the caller
 * will be blocked and appended to the dest's sending queue.
 * 
 * @param src	The caller, the sender.
 * @param dest	To whom the message is sent.
 * @param m	The message, belong to the memory space of the calling process.
 * 
 *****************************************************************************/
PUBLIC	void	msg_send(int src, int dest, MESSAGE*m)
{
	assert_kernel(src!=INTERRUPT && src>=0);
	assert_kernel(dest!=INTERRUPT && dest>=0);
	assert_kernel(src!=dest);
	assert_kernel(m);

	PROCESS_TABLE*current=pid2proc(src);
	PROCESS_TABLE*receiver=pid2proc(dest);

	/*
	if the receiver process's state is waiting, and the p_recvfrom is just the src process or ANY,
	then copy the message to receiver, and unblock it.
	*/
	if( (receiver->p_flags & RECEIVING) !=0 && (receiver->p_recvfrom==src||receiver->p_recvfrom==ANY)){
		/*
		Notie:
		Since receiver is waiting for receive,then receiver->msg must be pointing to some valid memory.
		So phys_copy() function copy the message to that memory. And the receiver process knows where 
		to find it.
		*/
		assert_kernel(receiver->msg);
		phys_copy(va2la(dest,receiver->msg),va2la(src,m),sizeof(MESSAGE));
		receiver->msg=0;
		receiver->p_flags &= ~RECEIVING;
		receiver->p_recvfrom=NO_TASK;
		if(__DEBUG_Ipc==1 && (src==__id || dest==__id) ){
			printf_kernel(" %d sent message to %d ",src,dest);
		}
		unblock(receiver);
	}
	/*
	The receiver process is not waiting for receive, or not receiving for the src process.
	Anyway, the sender process should be appended to the sending queue of the receiver process.
	And the sender should be blocker.
	*/
	else{
		if(__DEBUG_Ipc==1 && (src==__id || dest==__id) ){
			printf_kernel("%d receving from %d",dest,receiver->p_recvfrom);
			printf_kernel(" %d sending message to %d\n",src,dest);
		}	
		PROCESS_TABLE*p=receiver->sendingQ;
		if(p==0){
			receiver->sendingQ=current;
		}else{
			while(p->nextSendingPro!=0){p=p->nextSendingPro;}
			p->nextSendingPro=current;
		}
		/*
		Notie:
		Sender is waiting for send.
		When the receiver begin to receive from sender, it has to know the memory address of the message.
		"sender->msg=m;" sets the address so it can be found by receiver.
		*/
		current->msg=m;// can't use phys_copy here, because sender->msg itself doesn't own any memory space.
		m->source=src;
		current->p_sendto=dest;
		current->nextSendingPro=0;
		//current->p_flags |= SENDING;
		current->p_flags = current->p_flags | SENDING;
		assert_kernel(SENDING);
		assert_kernel( (current->p_flags & SENDING)!=0);
		block(current);
	}
	return;
}

/*****************************************************************************
 *                                msg_receive
 *****************************************************************************/
/**
 * <Ring 0> Try to get a message from the src proc. If src is blocked sending
 * the message, copy the message from it and unblock src. Otherwise the caller
 * will be blocked.
 * 
 * @param dest	The caller, the proc who wanna receive.
 * @param src	From whom the message will be received.
 * @param m	The message ptr to accept the message,belong to the memory space of the calling process.
 * 
 *****************************************************************************/
PROCESS_TABLE*inSendingQueue(PROCESS_TABLE*head,PROCESS_TABLE*sender);
PUBLIC	void	msg_receive(int dest, int src, MESSAGE*m)
{
	assert_kernel(dest!=src);
	assert_kernel(dest>INTERRUPT && dest>=0);
	assert_kernel(src>=INTERRUPT && src>=0);
	assert_kernel(m);
	PROCESS_TABLE*sender=0;
	PROCESS_TABLE*current=pid2proc(dest);
	PROCESS_TABLE*tmp=0;
	if(src==INTERRUPT){
		if(current->has_int_msg==TRUE){
			current->has_int_msg=FALSE;
			return;	
		}
	}
	if(src==ANY)/*from any process*/{
		if(__DEBUG_Ipc==1&&(dest==__id||src==__id)){
			printf_kernel("receive:ANY\n");
		}
		if(current->sendingQ!=0){
			sender=current->sendingQ;
			assert_kernel(sender);
			assert_kernel((sender->p_flags&SENDING)!=0);
			assert_kernel(sender->p_sendto==dest);
			src=proc2pid(sender);
			if(__DEBUG_Ipc==1 && (src==__id)){
				printf_kernel("receive:src changed from ANY to %d\n",src);
			}
		}
	}else{
		sender=pid2proc(src);
	}
	
	/*
	There is a process waiting to send message to this process
	*/
	if( sender!=0 && (sender->p_flags&SENDING)!=0 && sender->p_sendto==dest ){
		/*
		Notie:
		Since Sender is waiting for send.It must have set its "msg" as the message.
		You can check the msg_send function for reference.
		*/
		phys_copy( va2la(dest,m), va2la(src,sender->msg), sizeof(MESSAGE));
		sender->msg=0;
		sender->p_sendto=NO_TASK;
		sender->p_flags &=~SENDING;
		if(__DEBUG_Ipc==1 && (src==__id || dest==__id)){
			printf_kernel(" %d received message from %d ",dest,src);
		}
		unblock(sender);
		if(current->sendingQ==sender){
			current->sendingQ=sender->nextSendingPro;
		}else{
			tmp=inSendingQueue(current->sendingQ,sender);
			assert_kernel(tmp);
			assert_kernel(tmp->nextSendingPro==sender);
			tmp->nextSendingPro=sender->nextSendingPro;
		}
	}else{
		current->p_flags |= RECEIVING;
		current->p_recvfrom=src;
		current->msg=m;
		if(__DEBUG_Ipc==1 && (src==__id || dest==__id)){
			printf_kernel(" %d receiving message from %d ",dest,src);
		}
		block(current);
	}
	
}
/*****************************************************************************
 *                                inSendingQueue
 *****************************************************************************/
/**
 * <Ring 0> check whether a process is in sending queue,head excluded
 * 
 * @param head		Queue head
 * @param sender	the sender process
 *
 * @return 0 if not exist; otherwise return the previous element in the queue
 *****************************************************************************/
PUBLIC	PROCESS_TABLE*inSendingQueue(PROCESS_TABLE*head,PROCESS_TABLE*sender)
{
	while(head->nextSendingPro!=0){
		if(head->nextSendingPro==sender)
			return head;
		else
			head=head->nextSendingPro;
	}
	return 0;
}
/*****************************************************************************
 *                                inform_int
 *****************************************************************************/
/**
 * <Ring 0> inform a process that an interrupt has occured,always called by an interrupt handler
 * 
 * @param pid: the process_id
 *****************************************************************************/
PUBLIC void inform_int(int pid)
{
	PROCESS_TABLE*p=pid2proc(pid);
	assert_kernel(p!=0);
	/*the target process is waiting for a message*/
	printf_kernel("sending to %d ",pid);
	if( (p->p_flags&RECEIVING)!=0 && ( p->p_recvfrom==ANY|| p->p_recvfrom==INTERRUPT) ){
		p->has_int_msg=TRUE;
		p->p_flags&=~RECEIVING;
		unblock(p);
	}
	/*not receiving a message from INTERRUPT, 
	maybe waiting for another process
	maybe sending, maybe runnable*/
	else{
		p->has_int_msg=TRUE;
	}
}

