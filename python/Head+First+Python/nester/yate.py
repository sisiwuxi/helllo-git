from string import Template
#从标准库string模板导入Template类，支持简单的字符串替换模板#
def start_response(resp="text/html"):
    return('Content-type: ' + resp + '\n\n')
# 这个函数需要一个字符串作为参数，用来创建一个CGI行'Content-type: '，缺省值为"text/html"
def include_header(the_title):
    with open('templates/header.html') as headf:
        head_text=headf.read()
    header=Template(head_text)
    return(header.substitute(title=the_title))
#打开模板文件HTML读入文件换入所提供的标题；函数substitute需要一个字符串作为参数用在HTML页面最前面的标题中。页面本身存储在一个单独的文件templates/header.html中可以根据需要替换标题
def include_footer(the_links):
    with open('templates/footer.html') as footf:
        foot_text=footf.read()
    link_string=''
    for key in the_links:
        link_string+='<a href="' + the_links[key] + '">' + key + '</a>'
	#link_string+='<a href="' + the_links[key] + '">' + key + '</a>&nbsp;&nbsp;&nbsp;&nbsp;'
#“&nbsp;”是HTML在字符串中加入空格的一种强制做法
    footer=Template(foot_text)
    return(footer.substitute(links=link_string))
#打开模板读入文件换入the_links中提供的HTML链接字典；将链接字典转换为一个字符串再换入模板
def start_form(the_url, form_type="POST"):
    return('<form action="' + the_url + '" method="' + form_type + '">')
#返回表单最前面的HTML，允许调用者指定URL和使用的方法"POST"还是"GET"
def end_form(submit_msg="Submit"):
    return('<p></p><input type=submit value="' + submit_msg + '"></form>')
#返回表单末尾的HTML标记，允许调用者定制"Submit"按钮的文本
def radio_button(rb_name,rb_value):
    return('<input type="radio" name="' + rb_name +
            '"value="' + rb_value + '"> ' + rb_value + '<br />')
#给定一个单选钮名和值，创建一个HTML单选钮（通常包括在一个HTML表单中），注意两个参数都是必要的
def u_list(items):
    u_string='<ul>'
    for item in items:
        u_string += '<li>' + item + '</li>'
    u_string += '</ul>'
    return(u_string)
#给定一个项列表，这个函数会把该列表转换为一个HTML无序列表，每次向ul元素增加一个li元素
def header(header_text, header_level=2):
    return('<h' + str(header_level) + '>' + header_text + 
            '</h' + str(header_level) + '>')
#创建并返回一个HTML标题标记（H1，H2，H3等），默认为二级标题，“header_text”参数是必要的
def para(para_text):
    return('<p>' + para_text + '</p>')
#用HTML段落标记包围一个文本段（一个字符串）

#print(start_response())
#print(start_response("text/plain"))
#print(start_response("application/json"))
#print(include_header("Welcome to my home on the web!"))
#print(include_footer({}))
#print(include_footer({'Home':'/index.html','Select':'/cgi-bin/select.py'}))
#print(start_form("/process.py"))
print(end_form())
print(end_form("Click to confirm your order"))

