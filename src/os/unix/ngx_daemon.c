
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>

//// 实现守护进程
ngx_int_t
ngx_daemon(ngx_log_t *log)
{
    int  fd;

    //// fork子进程
    switch (fork()) {
    case -1:
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "fork() failed");
        return NGX_ERROR;

    case 0:
        //// 子进程继续往下执行
        break;

    default:
        //// 父进程直接退出 - 因为父进程不能实现守护进程
        exit(0);
    }


    //// 以下都是子进程的操作
    ngx_pid = ngx_getpid();

    //// 子进程创建会话id，实现守护进程
    //// 让父进程退出原因就是为了这一步，因为父进程是进程组组长，而进程组组长不能创建会话id
    if (setsid() == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "setsid() failed");
        return NGX_ERROR;
    }

    //// 设置文件掩码
    umask(0);


    //// 设置输入、输出文件描述符指向空洞文件
    fd = open("/dev/null", O_RDWR);
    if (fd == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      "open(\"/dev/null\") failed");
        return NGX_ERROR;
    }

    //// 输入
    if (dup2(fd, STDIN_FILENO) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "dup2(STDIN) failed");
        return NGX_ERROR;
    }

    //// 输出
    if (dup2(fd, STDOUT_FILENO) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "dup2(STDOUT) failed");
        return NGX_ERROR;
    }

#if 0
    if (dup2(fd, STDERR_FILENO) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "dup2(STDERR) failed");
        return NGX_ERROR;
    }
#endif

    if (fd > STDERR_FILENO) {
        if (close(fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "close() failed");
            return NGX_ERROR;
        }
    }

    return NGX_OK;
}
