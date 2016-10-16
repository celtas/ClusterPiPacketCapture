//
// Created by System Administrator on 9/23/16.
//

#ifndef CIT_MONOMAKE_PACKETCAPTURE_H
#define CIT_MONOMAKE_PACKETCAPTURE_H

#include <pcap.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>


#define PROMSCS_MODE  1
#define RCV_TIMEOUT   1000
#define DPCP_NOLIMIT_LOOP  -1

/* default snap length (maximum bytes per packet to capture) */
#define SNAP_LEN 1518

/* ethernet headers are always exactly 14 bytes [1] */
#define SIZE_ETHERNET 14
/* イーサネットアドレス（MACアドレス）は6バイト*/
#define ETHER_ADDR_LEN	6
#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)

/* イーサネットヘッダ */
struct sniff_ethernet {
    u_char ether_dhost[ETHER_ADDR_LEN]; /* 送信先ホストアドレス */
    u_char ether_shost[ETHER_ADDR_LEN]; /* 送信元ホストアドレス */
    u_short ether_type; /* IP? ARP? RARP? など */
};

/* IPヘッダ */
struct sniff_ip {
    u_char ip_vhl;		/* バージョン（上位4ビット）、ヘッダ長（下位4ビット） */
    u_char ip_tos;		/* サービスタイプ */
    u_short ip_len;		/* パケット長 */
    u_short ip_id;		/* 識別子 */
    u_short ip_off;		/* フラグメントオフセット */
    #define IP_RF 0x8000		/* 未使用フラグ（必ず0が立つ） */
    #define IP_DF 0x4000		/* 分割禁止フラグ */
    #define IP_MF 0x2000		/* more fragments フラグ */
    #define IP_OFFMASK 0x1fff	/* フラグメントビットマスク */
    u_char ip_ttl;		/* 生存時間（TTL） */
    u_char ip_p;		/* プロトコル */
    u_short ip_sum;		/* チェックサム */
    struct in_addr ip_src,ip_dst; /* 送信元、送信先IPアドレス */
};

/* TCP ヘッダー */
struct sniff_tcp {
    u_short th_sport;	/* 送信元ポート */
    u_short th_dport;	/* 送信先ポート */
    tcp_seq th_seq;		/* シーケンス番号 */
    tcp_seq th_ack;		/* 確認応答番号 */
    u_char th_offx2;	/* データオフセット、予約ビット */
    #define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
    u_char th_flags;
    #define TH_FIN 0x01
    #define TH_SYN 0x02
    #define TH_RST 0x04
    #define TH_PUSH 0x08
    #define TH_ACK 0x10
    #define TH_URG 0x20
    #define TH_ECE 0x40
    #define TH_CWR 0x80
    #define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
    u_short th_win;		/* ウインドサイズ */
    u_short th_sum;		/* チェックサム */
    u_short th_urp;		/* 緊急ポインタ */
};

class PacketCapture{
public:
    PacketCapture();
    static void start_pktfunc(u_char *user, const pcap_pkthdr *header, const u_char *packet);
private:

};

#endif // CIT_MONOMAKE_PACKETCAPTURE_H
