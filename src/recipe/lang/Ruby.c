/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-08-29>
 * Last Modified : <2024-11-08>
 * ------------------------------------------------------------*/

static MirrorSite
RubyChina = {"rubychina",    "RubyChina",    "Ruby China 社区",    "https://gems.ruby-china.com/",
             "https://gems.ruby-china.com/rubygems/gems/nokogiri-1.15.0-java.gem"}; // 9.9 MB

/**
 * @update 2024-09-04
 * @sync https://github.com/RubyMetric/chsrc/wiki/Ruby-MirrorSite
 * @sync https://github.com/RubyMetric/chsrc/discussions/62
 *
 * @note 曾经的问题 https://ruby-china.org/topics/43331
 */
static SourceInfo
pl_ruby_sources[] = {
  {&Upstream,     "https://rubygems.org"},
  {&RubyChina,    "https://gems.ruby-china.com/"},
  {&Ustc,         "https://mirrors.ustc.edu.cn/rubygems/"}

  // {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/rubygems/"},
  // {&Bfsu,      "https://mirrors.bfsu.edu.cn/rubygems/"},

  // {&Tencent,   "https://mirrors.tencent.com/rubygems/"},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/rubygems/"},
  // {&Ali,       "https://mirrors.aliyun.com/rubygems/"},
  // {&Huawei,    "https://mirrors.huaweicloud.com/repository/rubygems/"},
};
def_sources_n(pl_ruby);


void
pl_ruby_getsrc (char *option)
{
  chsrc_run ("gem sources", RunOpt_Default);
  chsrc_run ("bundle config get mirror.https://rubygems.org", RunOpt_Default);
}

void
pl_ruby_remove_gem_source (const char *source)
{
  char *cmd = NULL;
  if (is_url (source))
    {
      cmd = xy_str_delete_suffix (source, "\n");
      cmd = xy_2strjoin ("gem sources -r ", cmd);
      chsrc_run (cmd, RunOpt_Default);
    }
}

/**
 * @consult https://gitee.com/RubyMetric/rbenv-cn
 */
void
pl_ruby_setsrc (char *option)
{
  char *setsrc_type = xy_streql (option, SetsrcType_Reset) ? SetsrcType_Reset : SetsrcType_Auto;

  chsrc_ensure_program ("gem");

  chsrc_yield_source_and_confirm (pl_ruby);

  char *cmd = NULL;

  xy_run ("gem sources -l", 0, pl_ruby_remove_gem_source);

  cmd = xy_2strjoin ("gem source -a ", source.url);
  chsrc_run (cmd, RunOpt_Default);


  chsrc_ensure_program ("bundle");

  char *where = " --global ";
  if (CliOpt_Locally==true)
    {
      where = " --local ";
    }

  cmd = xy_strjoin (4, "bundle config", where, "'mirror.https://rubygems.org' ", source.url);
  chsrc_run (cmd, RunOpt_No_Last_New_Line);

  chsrc_conclude (&source, setsrc_type);
}

void
pl_ruby_resetsrc (char *option)
{
  pl_ruby_setsrc (SetsrcType_Reset);
}


FeatInfo
pl_ruby_feat (char *option)
{
  FeatInfo f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = PartiallyCan;
  f.cap_locally_explain = "Support `bundler`. No support for `gem`";
  f.can_english = true;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}


def_target_gsrf(pl_ruby);
