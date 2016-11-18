//
// Created by System Administrator on 9/23/16.
//

#include "PacketCapture.h"
#include <iostream>
#include "PortAPI.h"
#include "../database/DatabaseManager.h"

DatabaseManager *manager;
using namespace std;

PacketCapture::PacketCapture() {
    char *dev;
    char ebuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    dev = pcap_lookupdev(ebuf);
    dev = "eth0";
    if (dev == NULL) {
        fprintf(stderr, "ディバイスが見つかりませんでした: %s\n", ebuf);
        fprintf(stderr, "もしくは、root権限で実行してください!");
        exit(1);
    }

    cout << "ディバイス " << dev << " を読み込みました." << endl;

    //MysqlManagerの起動
    manager = new DatabaseManager();

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
    //DatabaseManager::closeDatabase();
}

void PacketCapture::start_pktfunc( u_char *user,       // pcap_loop関数の第4引数
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
        //printf("* 不正なIPヘッダ長: %u bytes\n", size_ip);
    }
    tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
    size_tcp = TH_OFF(tcp)*4;
    if (size_tcp < 20) {
        //printf("* 不正なTCPヘッダ長: %u bytes\n", size_tcp);
    }
    //ヘッダ部分を覗いたデータ本体
    payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);

    char src[32],dst[32];
    memcpy(src,inet_ntoa(ip->ip_src),32);
    memcpy(dst,inet_ntoa(ip->ip_dst),32);
    string dport = PortAPI::getService(tcp->th_dport);
    string sport = PortAPI::getService(tcp->th_sport);
    //if(dport != "none" || sport != "none") {
    string ssrc = src;
    string ddst = dst;

    //ローカルマシンの日付取得
    time_t timer;
    struct tm* tm;
    char datetime[20];
    timer = time(NULL);
    tm = localtime(&timer);
    strftime(datetime, 20, "%Y-%m-%d %H:%M:%S",tm );

    manager->insertPacket(ip->ip_src.s_addr,datetime,tcp->th_sport,tcp->th_dport);

//    printf("-------IPヘッダー-------\n");
//    printf("From : %s\n", src);
//    printf("To   : %s\n", dst);
//    printf("-------TCPヘッダー-------\n");
//
//    printf("送信元ポート: %s\n", &dport);
//    printf("送信先ポート: %s\n", &sport);

    //printf("送信先ポート: %s\n",PortAPI::getService(tcp->th_sport));
    //printf("シーケンス番号: %d\n",tcp->th_seq);
    //printf("確認応答番号: %d\n",tcp->th_ack);
//    printf("---------------------\n\n");
}