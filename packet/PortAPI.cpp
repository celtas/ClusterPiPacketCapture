//
// Created by System Administrator on 9/29/16.
//

#include "PortAPI.h"
#include <string>

string PortAPI::getService(int port) {
    string serviceName;

    switch (port) {
        case 20:
        case 21:
            serviceName = "FTP";
            break;
        case 22:
            serviceName = "ssh";
            break;
        case 23:
            serviceName = "Telnet";
            break;
        case 25:
            serviceName = "SMTP";
            break;
        case 42:
            serviceName = "WINS";
            break;
        case 43:
            serviceName = "WHOIS";
            break;
        case 53:
            serviceName = "DNS";
            break;
        case 67:
        case 68:
            serviceName = "BOOTP";
            break;
        case 69:
            serviceName = "TFTP";
            break;
        case 80:
            serviceName = "HTTP";
            break;
        case 88:
            serviceName = "Kerberos";
            break;
        case 109:
            serviceName = "POP2";
            break;
        case 110:
            serviceName = "POP3";
            break;
        case 113:
            serviceName = "ident";
            break;
        case 119:
            serviceName = "NNTP";
            break;
        case 123:
            serviceName = "NTP";
            break;
        case 135:
            serviceName = "Microsoft RPC";
            break;
        case 137:
        case 138:
        case 139:
            serviceName = "NetBIOS";
            break;
        case 143:
            serviceName = "IMAP2/4";
            break;
        case 161:
        case 162:
            serviceName = "SNMP";
            break;
        case 179:
            serviceName = "BGP";
            break;
        case 194:
            serviceName = "IRC";
            break;
        case 220:
            serviceName = "IMAP3";
            break;
        case 389:
            serviceName = "LDAP";
            break;
        case 443:
            serviceName = "HTTPS";
            break;
        case 445:
            serviceName = "Microsoft Directory Servise";
            break;
        case 465:
            serviceName = "SMTPS";
            break;
        case 514:
            serviceName = "syslog";
            break;
        case 520:
            serviceName = "RIP";
            break;
        case 546:
        case 547:
            serviceName = "DHCPv6";
            break;
        case 554:
            serviceName = "RTSP";
            break;
        case 587:
            serviceName = "SMTP";
            break;
        case 636:
            serviceName = "LDAPS";
            break;
        case 843:
            serviceName = "Adobe Flash";
            break;
        case 860:
            serviceName = "iSCSI";
            break;
        case 873:
            serviceName = "rsync";
            break;
        case 888:
            serviceName = "CDDB";
            break;
        case 989:
        case 990:
            serviceName = "FTPS";
            break;
        case 992:
            serviceName = "Telnets";
            break;
        case 993:
            serviceName = "IMAPS";
            break;
        case 995:
            serviceName = "POP3S";
            break;
        default:
            return "none";
            //return to_string(port);
    }

    return serviceName;
}