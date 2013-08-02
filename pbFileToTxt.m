function pbFileToTxt(fl)

filename=sprintf('%s.pb',fl);

fid = fopen(filename,'r');
dim = fread(fid,1,'uint');
for i = 1:dim
  d(i) = fread(fid,1,'uint');
end;
[B,count] = fread(fid,prod(d),'float');
B = reshape(B,fliplr(d));

fclose(fid);

fid=fopen(sprintf('%s_edge.txt',fl),'wb');
n=size(B,1);
fprintf(fid,'%d %d\n',n,size(B,2));
for i=1:n
     fprintf(fid,'%f ',B(i,:));
    fprintf(fid,'\n');
end
fclose(fid);

end