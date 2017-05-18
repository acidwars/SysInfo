#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <sys/sysinfo.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <libnet.h>


using namespace std;


passwd getInfo() {

    int s;
    struct passwd pwd;
    struct passwd *result;
    char *buf;
    size_t bufsize;
    uid_t uid = getuid();
    bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    buf = (char *) malloc(bufsize);
    if (!buf) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    s = getpwuid_r(uid, &pwd, buf, bufsize, &result);
    if (result == NULL) {
        exit(EXIT_FAILURE);
    }
    return pwd;

}


void createFile(const char * filename) {
    int fd;
    write(fd, "test", sizeof("test")-1);
    cout << "fd: " << fd;
    close(fd);
}

int main(int argc, char *argv[]) {
    const long minute = 60;
    const long hour = minute * 60;
    const long day = hour * 24;
    const double megabyte = 1024 * 1024;
    struct sysinfo si;
    struct ifaddrs *ifap, *ifa;
    getifaddrs(&ifap);
    sysinfo(&si);
    char info;
    passwd userInfo = getInfo();
    char hostname[10];
    gethostname(hostname, sizeof(hostname));
    struct sockaddr_in *sa;
    char *addr;
    /* Output! */
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr->sa_family == AF_INET) {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            cout << ifa->ifa_name << " " << addr << "\n";
        }
    }
    cout << "Hostname:\t\t" << hostname << "\n";
    cout << "UID:\t\t\t" << userInfo.pw_uid << "\n";
    cout << "Username:\t\t" << userInfo.pw_name << "\n";
    cout << "Home Directory:\t\t" << userInfo.pw_dir << "\n";
    cout << "Shell:\t\t\t" << userInfo.pw_shell << "\n";
    cout << "Total RAM:\t\t" << si.totalram / 1024 / 1024 << "MB\n";
    cout << "Free RAM:\t\t" << si.freeram / 1024 / 1024 << "MB\n";
}