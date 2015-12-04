ls=[]

while not STDIN.eof?
  l = STDIN.gets.chomp
  c = l.split.length
  ls << [c, l]
end
ls.sort_by! {|a| a[0]}

for a in ls
  begin
    puts a[1]
  rescue Errno::EPIPE
    break
  end
end
