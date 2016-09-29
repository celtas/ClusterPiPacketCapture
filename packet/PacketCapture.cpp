//
// Created by System Administrator on 9/23/16.
//

#include "PacketCapture.h"
#include <iostream>

using namespace std;

PacketCapture::PacketCapture() {
    char *dev;
    char ebuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    dev = pcap_lookupdev(ebuf);
    if (dev == NULL) {
        fprintf(stderr, "ディバイスが見つかりませんでした: %s\n", ebuf);
        fprintf(stderr, "もしくは、root権限で実行してください!");
        exit(1);
    }

    cout << "ディバイス: " << dev << "\n" << endl;

    handle = pcap_open_live(dev, SNAP_LEN, PROMSCS_MODE, RCV_TIMEOUT, ebuf);
    if (handle == NULL) {
        fprintf(stderr, "ディバイス「%s」を開けませんでした: %s\n", dev, ebuf);
        exit(1);
    }

    if(pcap_loop(handle,DPCP_NOLIMIT_LOOP,start_pktfunc,NULL) != 1 ){
        printf("例外がハンドルされました.");
    }

    //クローズ
    //pcap_close(handle);
}

void PacketCapture::start_pktfunc( u_char *user,                  // pcap_loop関数の第4引数
                    const struct pcap_pkthdr *header , // 受信したPacketの補足情報
                    const u_char *packet               // 受信したpacketへのポインタ
){
    //ヘッダー部分の参照
    const struct sniff_ethernet *ethernet;	/* イーサネットヘッダ */
    const struct sniff_ip *ip;		/* IPヘッダ */
    const struct sniff_tcp *tcp;		/* TCPヘッダ */
    const u_char *payload;			/* パケットペイロード */

    u_int size_ip;
    u_int size_tcp;

    ethernet = (struct sniff_ethernet*)(packet);
    ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    size_ip = IP_HL(ip)*4;
    if (size_ip < 20) {
        printf("* 不正なIPヘッダ長: %u bytes\n", size_ip);
    }
    tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
    size_tcp = TH_OFF(tcp)*4;
    if (size_tcp < 20) {
        printf("* 不正なTCPヘッダ長: %u bytes\n", size_tcp);
    }
    //ヘッダ部分を覗いたデータ本体
    payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);

    char src[32],dst[32];
    memcpy(src,inet_ntoa(ip->ip_src),32);
    memcpy(dst,inet_ntoa(ip->ip_dst),32);
    printf("-------IPヘッダー-------\n");
    printf("From : %s\n",src);
    printf("To   : %s\n",dst);
    printf("-------TCPヘッダー-------\n");
    printf("送信元ポート: %d\n",tcp->th_dport);
    //printf("送信先ポート: %d\n",tcp->th_sport);
    printf("シーケンス番号: %d\n",tcp->th_seq);
    printf("確認応答番号: %d\n",tcp->th_ack);
    printf("---------------------\n\n");
}