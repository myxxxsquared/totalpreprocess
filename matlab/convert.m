%%

files = dir('.');
filenum = length(files);

for i=1:filenum
    filename = files(i).name;
    if endsWith(filename, '.mat')
        load(filename)
        fid=fopen(strcat(filename, '.output'), 'w');
        
        polycountsize = size(polygt);
        polycount = polycountsize(1);
        for j=1:polycount
            t = polygt{j,6};
            xs = polygt{j,2};
            ys = polygt{j,4};
            lens = length(xs);
            fprintf(fid, "n %s\n", t);
            
            for k=1:lens
                fprintf(fid, "p %d, %d\n", xs(k), ys(k));
            end
            
            fprintf(fid, "e\n");
        end
        
        fclose(fid);
        
    end
end
