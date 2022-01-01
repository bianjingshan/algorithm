#include <stdio.h>

#include <glib.h>


static gint listen_port = 8852;
static gint num = 0;
static const gchar *listen_ip = "0.0.0.0";

static GOptionEntry entries[] =  
{
    {
        "listen",               /*参数长名*/
        'l',                    /*短参数名*/
        0,                      /*falg*/
        G_OPTION_ARG_STRING,    /*参数类型*/
        &listen_ip,              /*参数变量*/
        "Set lisetn ip",        /*选项说明*/
        "listen_ip"             /*参数选项占位符说明*/
    },

    {"port",'p',0,G_OPTION_ARG_INT,&listen_port,"listen port","port"},
};

static GOptionEntry sub_entries[] =  
{
    {"num",'n',0,G_OPTION_ARG_INT,&num,"test numt","num"},
};

int main(int argc ,char **argv)
{
    GError *error = NULL;
    GOptionContext *ctx = NULL;
#if 0
    ctx = g_option_context_new("-test tree model performance");
    g_option_context_add_main_entries(ctx,entries,NULL);
    g_option_context_set_summary(ctx," Glib Test by Andy\n mhpmii@126.com\n");
#else
    GOptionGroup *group = NULL;
    GOptionGroup *sub_group = NULL;
    guint i;

    ctx = g_option_context_new ("usage");
    group = g_option_group_new ("abc", "main group", "Show main help options", NULL, NULL);
    g_option_group_add_entries (group, entries);

    sub_group = g_option_group_new ("sub",  "sub group", "Show sub help options", NULL, NULL);
    g_option_group_add_entries (sub_group, sub_entries);

    g_option_context_set_main_group (ctx, group);
    g_option_context_add_group (ctx, sub_group);

#endif

    if(!g_option_context_parse(ctx,&argc,&argv,&error)) {
        g_print("option parseing failed : %s\n",error->message);
        return -1;
    }

    g_option_context_free(ctx);
    g_print("listen_ip: %s\n", listen_ip);
    g_print("listen_port: %d\n", listen_port);
    g_print("num: %d\n", num);
    return 0;

}
