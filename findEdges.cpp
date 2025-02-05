#include<bits/stdc++.h>
using namespace std;

#define JAI_SHREE_RAM  ios::sync_with_stdio(0); cin.tie(0);
const int len = 40;

typedef pair<double, double> POINT;
#define epb edgeList.push_back
#define vpb vertices.push_back


vector<pair<POINT,POINT>> edgeList;

vector<POINT> getvertices(void ){
    vector<int> start1,start2,end1,end2,selected;

    int s1,e1,s2,e2,sel;
    char c;

    while(cin>>s1>>c>>e1>>c>>s2>>c>>e2>>c>>sel){
        start1.push_back(s1);
        end1.push_back(e1);
        start2.push_back(s2);
        end2.push_back(e2);
        selected.push_back(sel);
    }

    // for(int i=0; i<start1.size(); i++){
    //     cout<<start1[i]<<" "<<end1[i]<<" "<<start2[i]<<" "<<end2[i]<<" "<<selected[i]<<endl;
    // }

    vector<POINT> vertices;

    int n = start1.size();

    int initial = 0;

    while(selected[initial] == 0){
        initial++;
    }

    int x = initial*len;
    POINT preStart, preEnd;

    if(selected[initial]==1){
        POINT A,B;
        A = {x-0.6,start1[initial]-0.6};
        B = {x-0.6,end1[initial]+0.6};
        vpb(A);
        vpb(B);
        epb({A,B});
        preStart = A;
        preEnd = B;
    }
    
    else if(selected[initial]==2){

        POINT A = {x+len-1+0.2,start2[initial]-0.6}; // right side start of small block
        POINT B = {x+len-1+0.2,end2[initial]+0.6}; // right side end of small block
        vpb(A);
        vpb(B);
        epb({A, B});

        // extreme are extended by 0.6 and others by 0.4
        POINT C,D,E,F;
        if(start2[initial] > end1[initial]){
            C = {x-0.6,start1[initial]-0.6};
            D = {x-0.6,end2[initial]+0.6};
            E = {x-0.4,start2[initial]-0.6};
            F = {x-0.4,end1[initial]+0.6};

            vpb(C);
            vpb(D);
            vpb(E);
            vpb(F);
            epb({C, D});
            epb({E, F});
            epb({B, D});
            epb({A, E});
            preStart = C;
            preEnd = F;
        }
        else{
            C = {x-0.6,start2[initial]-0.6};
            D = {x-0.6,end1[initial]+0.6};
            E = {x-0.4,start1[initial]-0.6};
            F = {x-0.4,end2[initial]+0.6};
            
            vpb(C);
            vpb(D);
            vpb(E);
            vpb(F);
            epb({C, D});
            epb({E, F});
            epb({F, B});
            epb({C, A});
            preStart = E;
            preEnd = D;
        }
    }

    int previndex = initial;

    bool iszero = 0;

    for(int i = initial+1;i<n;i++){
        int curr = selected[i];
        if(curr == 0){
            iszero = 1;
            continue;
        }

        int x = i*len;
        int prevx = previndex*len;

        if(iszero){
            iszero = 0;
            // previous large box's rightmost vertices and edges
            POINT X = {prevx+len-1+0.6,start1[previndex]-0.6};
            POINT Y = {prevx+len-1+0.6,end1[previndex]+0.4}; //0.2 for pipeline
            vpb(X);
            vpb(Y); 
            // previous box remaining 2 edges
            epb({X,Y});
            epb({X, preStart});


            if(curr == 2){

                // right side of small block
                POINT A = {x+len-1+0.2,start2[i]-0.6};
                POINT B = {x+len-1+0.2,end2[i]+0.6};
                vpb(A);
                vpb(B);
                epb({A,B});

                POINT C,D,E,F,G,H;

                if(end1[previndex] > max(end1[i],end2[i])){
                    C = {x-0.4,end1[previndex]+0.6};
                    D = {x-0.6,end1[previndex]+0.4};
                    E = {x-0.4, max(end1[i], end2[i]) + 0.6};
                    G = {x-0.4, min(end1[i], end2[i]) + 0.6};
                    F = {x-0.4,max(start1[i], start2[i])-0.6};
                    H = {x-0.6,min(start1[i], start2[i])-0.6};

                    vpb(C); vpb(D); vpb(E); vpb(F); vpb(G); vpb(H);
                    epb({F,G}); epb({C,E}); epb({D,H}); epb({D,Y}); epb({C,preEnd});

                    if(end2[i]>end1[i]){
                        epb({E,B}); epb({F,A});
                        // epb({E,A}); epb({F,B});
                        preEnd = G;
                        preStart = H;
                    }
                    else{
                        epb({G,B}); epb({H,A});
                        // epb({G,A}); epb({H,B});
                        preEnd = E;
                        preStart = F;
                    }

                }
                else if(end1[previndex] < min(start1[i],start2[i])){
                    C = {x-0.4,end1[previndex]+0.4};
                    D = {x-0.6,end1[previndex]+0.6};
                    E = {x-0.4,min(start1[i], start2[i]) -0.6};
                    F = {x-0.4,min(end1[i], end2[i]) +0.6};
                    G = {x-0.4,max(start1[i], start2[i]) -0.6};
                    H = {x-0.6,max(end1[i], end2[i])+0.6};
                    
                    vpb(C); vpb(D); vpb(E); vpb(F); vpb(G); vpb(H);
                    epb({D, preEnd});  epb({Y,C}); epb({E,C}); epb({D,H}); epb({G,F});

                    if(end2[i]>end1[i]){
                        // epb({A,H}); epb({B,G});
                        epb({B,H}); epb({A,G});
                        preEnd = F;
                        preStart = E;
                    }
                    else{
                        // epb({F,A}); epb({E,B});
                        epb({F,B}); epb({E,A});
                        preEnd = H;
                        preStart = G;
                    }

                }
                else{
                    C = {x-0.6,max(end1[i], end2[i])+0.6};
                    F = {x-0.6,min(start1[i], start2[i])-0.6};
                    E = {x-0.4,min(end1[i], end2[i])+0.6};
                    D = {x-0.4,max(start1[i],start2[i])-0.6};
                    H = {x-0.6,end1[previndex]+0.6};
                    G = {x-0.6,end1[previndex]+0.4};
                    
                    vpb(C); vpb(D); vpb(E); vpb(F); vpb(G); vpb(H);
                    epb({preEnd,H}); epb({Y,G}); epb({H,C}); epb({F,G}); epb({D,E}); 

                    if(end2[i]>end1[i]){
                        // epb({C,A}); epb({D,B});
                        epb({C,B}); epb({D,A});
                        preEnd = E;
                        preStart = F;
                    }
                    else{
                        // epb({E,A}); epb({F,B});
                        epb({E,B}); epb({F,A});
                        preEnd = C;
                        preStart = D;
                    }

                }

            }

            if(curr == 1){
                POINT C,D,E,F;
                if(end1[previndex] > end1[i]){
                    C = {x-0.6,end1[previndex]+0.4};
                    D = {x-0.4,end1[previndex]+0.6};
                    E = {x-0.4,end1[i]+0.6};
                    F = {x-0.6,start1[i]-0.6};

                    vpb(C); vpb(D); vpb(E); vpb(F);
                    epb({preEnd,D}); epb({Y,C}); epb({D,E}); epb({C,F});
                    preEnd = E;
                    preStart = F;
                }
                else if(end1[previndex] < start1[i]){
                    C = {x-0.4,end1[previndex]+0.4};
                    D = {x-0.6,end1[previndex]+0.6};
                    E = {x-0.4,start1[i]-0.6};
                    F = {x-0.6,end1[i]+0.6};

                    vpb(C); vpb(D); vpb(E); vpb(F);
                    epb({preEnd,D}); epb({Y,C}); epb({C,E}); epb({D,F});
                    preEnd = F;
                    preStart = E;
                }
                else{
                    C = {x-0.6,end1[i]+0.6};
                    D = {x-0.6,end1[previndex]+0.6};
                    E = {x-0.6,end1[previndex]+0.4};
                    F = {x-0.6,start1[i]-0.6};
                    
                    vpb(C); vpb(D); vpb(E); vpb(F);
                    epb({preEnd,D}); epb({Y,E}); epb({D,C}); epb({E,F});
                    preEnd = C;
                    preStart = F;
                }
            }

        }
        
        else{
            if(curr==1){
                POINT C,D,E,F;

                if(start1[previndex] == start1[i] && (end1[previndex] == end1[i])){
                    previndex = i;
                    continue;
                }

                if(start1[previndex] == start1[i]){
                    if(end1[i]>end1[previndex]){
                        C = {x-0.6, end1[i]+0.6};
                        D = {x-0.6, end1[previndex]+0.6};
                    }
                    else{
                        C = {x-0.4, end1[i]+0.6};
                        D = {x-0.4, end1[previndex]+0.6};
                    }
                    vpb(C); vpb(D);
                    epb({C,D}); epb({preEnd,D});
                    preEnd = C;

                }
                else if(end1[previndex] == end1[i]){
                    if(start1[i]>start1[previndex]){
                        C = {x-0.4, start1[i]-0.6};
                        D = {x-0.4, start1[previndex]-0.6};
                    }
                    else{
                        C = {x-0.6, start1[i]-0.6};
                        D = {x-0.6, start1[previndex]-0.6};
                    }
                    vpb(C); vpb(D);
                    epb({C,D}); epb({D, preStart});
                    preStart = C;
                }
                else{
                    if(end1[i] > end1[previndex]){
                        C = {x-0.6, end1[i]+0.6};
                        D = {x-0.6, end1[previndex]+0.6};

                        if(start1[i] > start1[previndex]){
                            E = {x-0.4, start1[i]-0.6};
                            F = {x-0.4, start1[previndex]-0.6};
                        }
                        else{
                            E = {x-0.6, start1[i]-0.6};
                            F = {x-0.6, start1[previndex]-0.6};
                        }

                        vpb(C); vpb(D); vpb(E); vpb(F);
                        epb({C,D}); epb({E,F}); epb({preEnd,D}); epb({preStart,F});
                        preStart = E;
                        preEnd = C;
                    }
                    else{
                        C = {x-0.4, end1[previndex]+0.6};
                        D = {x-0.4, end1[i]+0.6};

                        if(start1[i] > start1[previndex]){
                            E = {x-0.4, start1[previndex]-0.6};
                            F = {x-0.4, start1[i]-0.6};
                        }
                        else{
                            E = {x-0.6, start1[previndex]-0.6};
                            F = {x-0.6, start1[i]-0.6};
                        }

                        vpb(C); vpb(D); vpb(E); vpb(F);
                        epb({C,D}); epb({E,F}); epb({preEnd,C}); epb({preStart,E});
                        preStart = F;
                        preEnd = D;
                    }

                }

            }

            else{
                // curr = 2;
                POINT A,B,C,D,E,F,G,H;
                A = {x+len-1+0.2, start2[i]-0.6};
                B = {x+len-1+0.2, end2[i]+0.6};

                C = {x-0.4, max(start1[i], start2[i]) -0.6};
                D = {x-0.4, min(end1[i], end2[i]) +0.6};

                vpb(A); vpb(B); vpb(C); vpb(D);
                epb({A,B}); epb({C,D});

                double newStart = min(start1[i], start2[i]);
                double newEnd = max(end1[i], end2[i]);

                if(newEnd > end1[previndex]){
                    E = {x-0.6, newEnd+0.6};
                    F = {x-0.6, end1[previndex]+0.6};

                    if(newStart > start1[previndex]){
                        G = {x-0.4, newStart-0.6};
                        H = {x-0.4, start1[previndex]-0.6};
                    }
                    else{
                        G = {x-0.6, newStart-0.6};
                        H = {x-0.6, start1[previndex]-0.6};
                    }
                    vpb(E); vpb(F); vpb(G); vpb(H);
                    epb({preEnd,F}); epb({preStart,H});  epb({G,H}); epb({E,F});
                    
                    if(end2[i]>end1[i]){
                        epb({E,B}); epb({C,A});
                        preStart = G;
                        preEnd = D;
                    }
                    else{
                        epb({D,B}); epb({G,A});
                        preStart = C;
                        preEnd = E;
                    }
                }
                else{
                    E = {x-0.4, end1[previndex]+0.6};
                    F = {x-0.4, newEnd+0.6};

                    if(newStart > start1[previndex]){
                        G = {x-0.4, newStart-0.6};
                        H = {x-0.4, start1[previndex]-0.6};
                    }
                    else{
                        G = {x-0.6, newStart-0.6};
                        H = {x-0.6, start1[previndex]-0.6};
                    }

                    vpb(E); vpb(F); vpb(G); vpb(H);
                    epb({preEnd,E}); epb({preStart,H});  epb({G,H}); epb({E,F});

                    if(end2[i]>end1[i]){
                        epb({F,B}); epb({C,A});
                        preStart = G;
                        preEnd = D;
                    }
                    else{
                        epb({D,B}); epb({G,A});
                        preStart = C;
                        preEnd = F;
                    }


                }

            }
        }

        previndex = i;
    }
    POINT lastS, lastE;
    lastS = {10000, start1[previndex]-0.6};
    lastE = {10000, end1[previndex]+0.6};
    epb({lastE, lastS});
    epb({preEnd, lastE});
    epb({preStart, lastS});

    vpb(lastE); vpb(lastS);

    return vertices;

}

int main(){
    JAI_SHREE_RAM

    freopen("subarrays.txt", "r", stdin);

    vector<POINT> vertices = getvertices();

    ofstream file("vertices.txt");

    for(int i=0; i<vertices.size(); i++){
        file << vertices[i].first<<" "<<vertices[i].second<<endl;
    }
    ofstream edgeOut("edges.txt");
    for (int i = 0; i < edgeList.size(); i++)
    {
        edgeOut << edgeList[i].first.first<<" "<<edgeList[i].first.second<<" "<<
        edgeList[i].second.first<<" "<<edgeList[i].second.second<<endl;

    }
     


    

    return 0;
}