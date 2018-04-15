#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define ITERATION_NUM 100000
int num_edge = 0;
int num_node = 0;
int num_color = 5;

int **t_adjacent_half=NULL; //记录节点的比自身序号小的邻边
int **t_adjacent_all = NULL; //记录节点的所有邻边 t_adjacent_all[i][0]表示总数

int *l_conflict_num = NULL;

int *l_s_curr = NULL;
int **t_conflict_tabel = NULL;
int **t_tabu_tenure = NULL;
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
    int delta_nontabu=0;
    int delta_tabu=0;
    int temp_delta;
    int best_conflict = num_node*num_node;
    int *best_solution = (int*)malloc(sizeof(int)*num_node);
    // 初始化
    for(i=0;i<num_node;i++){
        best_solution[i]=solution[i];
        for(j=0;i<num_color;j++){
            t_conflict_tabel[i][j]=0;
            t_tabu_tenure[i][j]=0;
        }
    }
    // 构建冲突表
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
    // TS algorithm
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
            printf("you success");
            break;
        }
        *conflict_num = best_conflict;
        for(i=0;i<num_node;i++){
            solution[i]=best_solution[i];
        }
    }
}
void read_file(void)
{
    int i=0;
    int j=0;
    int d_line_longth = 120;
    int *is_list_adjacents;  //记录每个节点有多少邻边
    int i_node_one;
    int i_node_two;
    char *cs_temp = NULL;
    char *cs_line = (char*)malloc(sizeof(char)*d_line_longth);
    FILE *f_instance=NULL;

    if(NULL==(f_instance=fopen("/home/fanzhikang/桌面/instances/DSJC125.1.col","r"))){
        printf("read file error!");
        exit(-1);
    }
    while(NULL!=fgets(cs_line,d_line_longth,f_instance)){
        if('p'==cs_line[0])
        {
            cs_temp=strtok(cs_line," ");
            cs_temp=strtok(NULL," ");
            cs_temp=strtok(NULL," ");
            num_node = atoi(cs_temp);
            cs_temp = strtok(NULL," ");
            
            num_edge = atoi(cs_temp);
            break;
        }
    }
    is_list_adjacents = (int*)malloc(sizeof(int)*num_node);
    is_list_adjacents[0]=0;
    t_adjacent_half = (int**)malloc(sizeof(int*)*num_node);
    t_adjacent_all = (int**)malloc(sizeof(int*)*num_node);
    j=0; //记录进行到的节点序号
    while(NULL!=fgets(cs_line,d_line_longth,f_instance)){
        cs_temp=strtok(cs_line," ");
        cs_temp=strtok(NULL," ");
        i_node_one = atoi(cs_temp)-1;
        cs_temp=strtok(NULL," ");
        i_node_two = atoi(cs_temp)-1;
        if(j==i_node_one){
            is_list_adjacents[0]++;
            is_list_adjacents[is_list_adjacents[0]]=i_node_two;
        }
        else{
            t_adjacent_half[j]=(int*)malloc(sizeof(int)*(is_list_adjacents[0]+1));
            t_adjacent_half[j][0]=is_list_adjacents[0];
            for(int i =0;i<is_list_adjacents[0];i++)
            {
                t_adjacent_half[j][i+1]=is_list_adjacents[i+1];
            }
            j++;
            while(j<i_node_one){
                t_adjacent_half[j]=(int*)malloc(sizeof(int)*1);
                t_adjacent_half[j][0]=0;
                j++;
            }
            is_list_adjacents[0]=1;
            is_list_adjacents[1]=i_node_two;
        }
    }
    t_adjacent_half[j]=(int*)malloc(sizeof(int)*(is_list_adjacents[0]+1));
    t_adjacent_half[j][0]=is_list_adjacents[0];
    for(int i=0;i<is_list_adjacents[0];i++){
        t_adjacent_half[j][i+1]=is_list_adjacents[i+1];
    }
    fclose(f_instance);
    if(num_node!=j+1){
        printf("j<num_node, the graph may be wrong");
        exit(-1);
    }
    // 利用得到的半邻边表，先算出每个节点的邻边数
    for(i=0;i<num_node;i++){
        is_list_adjacents[i]=0;
    }
    for(i=0;i<num_node;i++){
        for(j=0;j<t_adjacent_half[i][0];j++){
            is_list_adjacents[i]++;
            is_list_adjacents[t_adjacent_half[i][j+1]]++;
        }
    }
    // for(i=0;i<num_node;i++){
    //     for(j=0;j<t_adjacent_half[i][0];j++){
    //         printf("%d  %d  %d\n",i,is_list_adjacents[i],is_list_adjacents[t_adjacent_half[i][j+1]]);
    //     }
    // }
    for(i=0;i<num_node;i++){
        t_adjacent_all[i]=(int*)malloc(sizeof(int)*(is_list_adjacents[i]+1));
        t_adjacent_all[i][0]=0;
    }
    for(i=0;i<num_node;i++){
        for(j=0;j<t_adjacent_half[i][0];j++){
            t_adjacent_all[i][0]++;
            t_adjacent_all[i][t_adjacent_all[i][0]]=t_adjacent_half[i][j+1];
            t_adjacent_all[t_adjacent_half[i][j+1]][0]++;
            t_adjacent_all[t_adjacent_half[i][j+1]][t_adjacent_all[t_adjacent_half[i][j+1]][0]]=i;
        }
    }
    // for(i=0;i<num_node;i++)
    // printf("%d %d\n",i,t_adjacent_all[i][1]);
}

void initial_population(void)
{
    int i;
    srand((unsigned int)time(NULL));
    //全局变量初始化
    l_conflict_num = (int*)malloc(sizeof(int));
    l_s_curr=(int*)malloc(sizeof(int)*num_node);
    t_conflict_tabel = (int**)malloc(sizeof(int *)*num_node);
    for(i=0;i<num_node;i++){
        t_conflict_tabel[i]=(int*)malloc(sizeof(int)*num_color);
    }
    t_tabu_tenure=(int**)malloc(sizeof(int*)*num_node);
    for(i=0;i<num_node;i++){
        t_tabu_tenure[i]=(int*)malloc(sizeof(int)*num_color);
    }
    for(i=0;i<num_node;i++){
        l_s_curr[i]=rand()%num_color;
    }
    tabu_search(l_s_curr,&l_conflict_num[0]);
}
void free_variable(void)
{
    

    int i;
    int j;
    for(i=0;i<num_node;i++){
        free(t_adjacent_half[i]);
        free(t_adjacent_all[i]);
        free(t_conflict_tabel[i]);
        free(t_tabu_tenure[i]);
    }
    free(t_adjacent_half);
    free(t_adjacent_all);
    free(t_conflict_tabel);
    free(t_tabu_tenure);

    free(l_conflict_num);

    free(l_s_curr);
}

int main()
{
    read_file();
    initial_population();
    free_variable();
}