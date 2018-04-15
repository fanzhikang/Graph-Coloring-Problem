# Graph-Coloring-Problem

**禁忌搜索**

```c
void tabu_search(int *solution, int *conflict_num)
{
    int i;
    int j;
    int m;
    int temp_node;
    int temp_color;
    int color_before;
    int color_after;
    int record_nontabu[2];
    int record_tabu[2];
    int choice_num_tabu;
    int choice_num_nontabu;
    int curr_conflict_num=0;
    int delta_nontabu = 0;
    int delta_tabu = 0;
    int temp_delta = 0;
    int best_conflict = num_node*num_node;
    int *best_solution = (int*)malloc(sizeof(int)*num_node);
    //
    for(i=0;i<num_node;i++){
        best_solution[i]=solution[i];
        for(j=0;i<num_color;j++){
            t_conflict_tabel[i][j]=0;
            t_tabu_tenure[i][j]=0;
        }
    }
    //
    for(i=0;i<num_node;i++){
        for(j=0;j<t_adjacent_all[i][0];j++){
            temp_node = t_adjacent_all[i][j+1];
            temp_color = solution[i];
            t_conflict_tabel[temp_node][temp_color]++;
        }
    }
    for(i=0;i<num_node;i++){
        temp_node=i;
        temp_color=solution[temp_node];
        curr_conflict_num+=t_conflict_tabel[temp_node][temp_color];
    }
    curr_conflict_num/=2;
    //TS Algorithm
    for(i=0;j<ITERATION_NUM;i++){
        delta_nontabu=num_node;
        delta_tabu=num_node;
        choice_num_tabu=0;
        choice_num_nontabu=0;
        for(j=0;j<num_node;j++){
            temp_node=j;
            temp_color=solution[temp_node];
            if(0<t_conflict_tabel[temp_node][temp_color]){
                for(m=1;m<num_color;m++){
                    color_before=temp_color;
                    color_after=(temp_color+m)%num_color;
                    temp_delta = t_conflict_tabel[temp_node][color_after]-t_conflict_tabel[temp_node][color_before];
                    //对禁忌和非禁忌的情况分别处理
                    if(t_tabu_tenure[temp_node][color_after]<i){
                        if(temp_delta<delta_nontabu){
                            record_nontabu[0]=temp_node;
                            record_nontabu[1]=color_after;
                            choice_num_nontabu=1;
                            delta_nontabu=temp_delta;
                        }
                        else{
                            if(delta_nontabu==temp_delta){
                                choice_num_nontabu++;
                                if(0==rand()%choice_num_nontabu){
                                    record_nontabu[0]=temp_node;
                                    record_nontabu[1]=color_after;
                                }
                            }
                        }
                    }
                    else{
                        if(temp_delta<delta_tabu){
                            record_tabu[0]=temp_node;
                            record_tabu[1]=color_after;
                            choice_num_tabu=1;
                            delta_tabu=temp_delta;
                        }
                        else{
                            if(delta_tabu==temp_delta){
                                choice_num_tabu++;
                                if(0==rand()%choice_num_tabu){
                                    record_tabu[0]=temp_node;
                                    record_tabu[1]=color_after;
                                }
                            }
                        }
                    }
                }
            }
        }
        //选择好的移动方法
        if((curr_conflict_num+delta_tabu<best_conflict && delta_tabu<delta_nontabu)||(choice_num_nontabu==0)){
            temp_node=record_tabu[0];
            color_after=record_tabu[1];
            temp_delta=delta_tabu;
        }
        else{
            temp_node=record_nontabu[0];
            color_after=record_nontabu[1];
            temp_delta=delta_nontabu;
        }
        color_before=solution[temp_node];
        solution[temp_node]=color_after;
        curr_conflict_num+=temp_delta;
        for(m=0;m<t_adjacent_all[temp_node][0];m++){
            t_conflict_tabel[t_adjacent_all[temp_node][m+1]][color_before]--;
            t_conflict_tabel[t_adjacent_all[temp_node][m+1]][color_after]++;
        }
        t_tabu_tenure[temp_node][color_before]=i+curr_conflict_num+rand()%10;
        if(curr_conflict_num<=best_conflict){
            for(m=0;m<num_node;m++){
                best_solution[m]=solution[m];
            }
            best_conflict=curr_conflict_num;
        }
        if(best_conflict==0){
            //printf("you success");
            break;
        }
        *conflict_num = best_conflict;
        for(i=0;i<num_node;i++){
            solution[i]=best_solution[i];
        }
    }
}
```

### 关键部分

- 求冲突数

~~~c
for(i=0;i<num_node;i++){
        temp_node=i;
        temp_color=solution[temp_node];
        curr_conflict_num+=t_conflict_tabel[temp_node][temp_color];
    }
    curr_conflict_num/=2; //除2
~~~

- 搜索邻域

~~~c
for(m=1;m<num_color;m++){
                    color_before=temp_color;
                    color_after=(temp_color+m)%num_color;
                    temp_delta = t_conflict_tabel[temp_node][color_after]-t_conflict_tabel[temp_node][color_before];
~~~

- 更新冲突表

~~~c
for(m=0;m<t_adjacent_all[temp_node][0];m++){
            t_conflict_tabel[t_adjacent_all[temp_node][m+1]][color_before]--;
            t_conflict_tabel[t_adjacent_all[temp_node][m+1]][color_after]++;
        }
~~~

