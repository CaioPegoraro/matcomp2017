using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace trab1Simplex
{
    public partial class Form1 : Form
    {
        List<String> listaRestricoes = new List<String>();

        int contador = 1;
        TextBox[] txtRestricoes;
        TextBox[] txtResultados;
        TextBox[] txtLinhaZero;

        Label[] labels, labels2, labels3, labels4;
        int n = 3;

        int nRestricoes;
        int nVariaveis;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {


        }

        private void btnAdicionar_Click(object sender, EventArgs e)
        {

            btnAdicionar.Enabled = false;
            
            /*
            String txt_temp1 = this.txtEquacao.Text.ToString();
            String txt_temp2 = this.txtResultado.Text.ToString();

            String txt_temp3 = txt_temp1 + txt_temp2;

            listaRestricoes.Add(txt_temp3);
            txtEquacao.Text = "";
            txtResultado.Text = "";

            txtRestricoes.AppendText("\n" + txt_temp1 + " <= " + txt_temp2 + "\n");
            */

            nRestricoes = int.Parse(txtNrestricoes.Text);
            nVariaveis = int.Parse(txtNvariaveis.Text);

            txtNrestricoes.ReadOnly = true;
            txtNvariaveis.ReadOnly = true;

            int x = 50;
            int y = 150;
            n = nVariaveis * nRestricoes;

            txtRestricoes = new TextBox[n];
            txtResultados = new TextBox[nRestricoes];
            txtLinhaZero = new TextBox[nVariaveis];

            labels = new Label[n];
            labels2 = new Label[n];
            labels3 = new Label[n];
            labels4 = new Label[nVariaveis];

            for (int i = 0; i < nRestricoes; i++)
            {
                txtResultados[i] = new TextBox();
                labels[i] = new Label();
                labels3[i] = new Label();

            }

            for (int j = 0; j < n; j++)
            {
                txtRestricoes[j] = new TextBox();
                labels2[j] = new Label();
            }

            for (int k = 0; k < nVariaveis; k++)
            {
                txtLinhaZero[k] = new TextBox();
                labels4[k] = new Label();
            }

            n = 80;

            for (int k = 0; k < nVariaveis; k++)
            {
                this.Controls.Add(txtLinhaZero[k]);
                txtLinhaZero[k].Location = new Point(n, 92);
                txtLinhaZero[k].Size = new System.Drawing.Size(30, 30);
                txtLinhaZero[k].BringToFront();
                n += 30;

                this.Controls.Add(labels4[k]);
                labels4[k].Location = new Point(n, 95);
                labels4[k].Text = "X" + k.ToString();
                labels4[k].BringToFront();

                n += 30;

            }

            n = 0;

            for (int i = 0; i < nRestricoes; i++)
            {
                    this.Controls.Add(labels[i]);
                    labels[i].Location = new Point(x, y);
                    labels[i].Text = i.ToString();
                    labels[i].BringToFront();
                    x += 30;

                int tmp = 0;

                for (int j = 0; j < nVariaveis; j++)
                {
                    
                    this.Controls.Add(txtRestricoes[n]);
                    y -= 2;
                    txtRestricoes[n].Location = new Point(x, y);
                    txtRestricoes[n].BringToFront();
                    txtRestricoes[n].Text = "0";
                    txtRestricoes[n].Size = new System.Drawing.Size(30, 30);
                    y += 2;

                    x += 30;
                    this.Controls.Add(labels2[n]);
                    labels2[n].Location = new Point(x, y);
                    labels2[n].Text = "X" + tmp.ToString();
                    labels2[n].BringToFront();

                    tmp++;
                    if (tmp == nVariaveis)
                        tmp = 0;

                    x += 45;
                    n++;
                }

                x -= 10;

                this.Controls.Add(labels3[i]);
                labels3[i].Location = new Point(x, y);
                labels3[i].Text = "<=";
                labels3[i].BringToFront();

                x += 20;

                y -= 2;
                this.Controls.Add(txtResultados[i]);
                txtResultados[i].Location = new Point(x, y);
                txtResultados[i].Size = new System.Drawing.Size(30, 24);
                txtResultados[i].BringToFront();
                y += 2;

                x = 50;
                y += 30;

            }

            //macro para teste rapido
/*
            txtResultados[0].Text = "3";
            txtResultados[1].Text = "4";
            txtResultados[2].Text = "9";

            txtLinhaZero[0].Text = "5";
            txtLinhaZero[1].Text = "2";

            txtRestricoes[0].Text = "1";
            txtRestricoes[1].Text = "0";
            txtRestricoes[2].Text = "0";
            txtRestricoes[3].Text = "1";
            txtRestricoes[4].Text = "1";
            txtRestricoes[5].Text = "2";
            */
        }

        private void btnVerificar_Click(object sender, EventArgs e)
        {
            //montar a matriz para aplicar o método simplex

            //precisa: - adicionar espaço para variaveis basicas (novas colunas)

            int nVariaveisFolga = nRestricoes;
            int nColunas = nVariaveis + nVariaveisFolga + 2; //+1 da variavel Z e +1 da constante
            int nLinhas = nRestricoes + 1; //+1 da linha zero

            float[,] matrizSimplex;
            matrizSimplex = new float[nLinhas, nColunas];

            int[] varBase;
            varBase = new int[nVariaveisFolga];

            for (int i = 0; i < nVariaveisFolga; i++)
            {
                varBase[i] = nVariaveis + i;
            }

            //preencher matriz simplex

            //linha zero
            for (int i = 0; i < nLinhas; i++)
            {
                if (i == 0)
                    matrizSimplex[0, i] = 1;
                else if (i < nVariaveis+1)
                    matrizSimplex[0, i] = (-1)*int.Parse(txtLinhaZero[i-1].Text); //-1 para deixar na forma canonica: Z - aX0 - bX1 = 0
                else
                    matrizSimplex[0, i] = 0;
            }
            
            //preencher variaveis nao basicas
             int k = 0;
                for (int i = 1; i < nLinhas; i++)
                {
                    for (int coluna = 1; coluna <= nVariaveis; coluna++)
                    {
                        matrizSimplex[i, coluna] = int.Parse(txtRestricoes[k].Text);
                        k++;
                    }
                    
                }

            int inX = 1;
            int inY = nVariaveis + 1;
            //preencher variáveis básicas
            for (int i = 1; i < nLinhas; i++)
            {
                for (int j = nVariaveis+1; j < nColunas; j++)
                {
                    if (j - i == 2)
                    {
                        matrizSimplex[i, j] = 1; //marca o indice da variavel basica como 1
                    }
                }
            }


            //preencher coluna constantes
             for (int i = 1; i < nLinhas; i++)
             {
                matrizSimplex[i, nColunas-1] = int.Parse(txtResultados[i-1].Text);
             }
                 
            String temp = "";
            for (int i = 0; i < nLinhas; i++)
            {
                for (int j = 0; j < nColunas; j++)
                {
                    temp += matrizSimplex[i,j].ToString() + " ";
                }
                listBox1.Items.Add(temp);
                temp = "";
            }

            //Fim etapa de input dos dados do problema

            //inicio algoritmo de resolução simplex

            //matrizSimplex[i,j]
            //nColunas,nLinhas
            //varBase[k] : variaveis base

            int terminou = 0;
            int colunaPivo = 1, linhaPivo = 1;
            float numeroPivo = 1;

            while (terminou==0)
            {
                //determinar coluna pivo achando o coeficiente negativo na linha zero
                int achou = 0;
                for (int i = 0; i < nColunas; i++)
                {
                    if (matrizSimplex[0, i] < 0)
                    {
                        achou = 1;
                        colunaPivo = i;
                        break;//primeiro negativo
                    }
                }
                if (achou == 0)
                {
                    terminou = 1;
                    break;
                }

                //determinar a linha pivo achando a menor razao positiva entre cte e numero na coluna pivo
                float razaoTemp=100000,razaoTemp2=10000;
                numeroPivo = 10000;

                for (int i = 1; i < nLinhas; i++)
                {
                    
                    if (matrizSimplex[i, colunaPivo] == 0)
                        razaoTemp = 99999999;
                    else
                        razaoTemp = matrizSimplex[i, nColunas - 1] / matrizSimplex[i, colunaPivo];
                    if ((razaoTemp > 0) && (razaoTemp < razaoTemp2))
                    {
                        razaoTemp2 = razaoTemp;
                        linhaPivo = i;
                    }
                }
                numeroPivo = matrizSimplex[linhaPivo, colunaPivo];

                temp = "=== Analise Pivo ===";
                listBox1.Items.Add(temp);
                temp = "";

                temp += "linhaP: " + linhaPivo.ToString() + " colunaP: " + colunaPivo.ToString() + " nPivo: " + numeroPivo.ToString();
                listBox1.Items.Add(temp);

                //determinado coluna, linha e numero pivo
                
                //editar base: sai o elemento da linha pivo, entra o elemento da coluna pivo
                varBase[linhaPivo-1] = colunaPivo-1;

                //gerar nova linha pivo
                for (int i = 0; i < nColunas; i++)
                {
                    matrizSimplex[linhaPivo, i] = matrizSimplex[linhaPivo, i] / numeroPivo;
                }

                //gerar novas linhas
                float coefPivo;

                for (int i = 0; i < nLinhas; i++)
                {
                    if (i != linhaPivo)
                    {
                        coefPivo = matrizSimplex[i, colunaPivo];
                        for (int j = 0; j < nColunas; j++)
                        {
                            matrizSimplex[i, j] = matrizSimplex[i, j] - coefPivo * matrizSimplex[linhaPivo, j];
                        }
                    }
                } 

                temp = "=== " + contador + " interacao Simplex ===";
                contador++;
                listBox1.Items.Add(temp);
                temp = "";

                for (int i = 0; i < nLinhas; i++)
                {
                    for (int j = 0; j < nColunas; j++)
                    {
                        temp += matrizSimplex[i, j].ToString() + " ";
                    }
                    listBox1.Items.Add(temp);
                    temp = "";
                }
            }

            //terminou o algoritmo: precisa exibir os resultados

            String[] resultado = new String[1+nVariaveis+nVariaveisFolga];

            Label[] lblResultados = new Label[1+nVariaveis+nVariaveisFolga];

            for (int i = 0; i < nVariaveisFolga; i++)
            {
                resultado[i]= "X" + varBase[i].ToString() + " = " + matrizSimplex[i + 1, nColunas - 1].ToString();
            }


            int qntVar = nVariaveisFolga + nVariaveis;
            int ktmp = 0;

            for (int i = 0; i < qntVar; i++)
            {
                int achou = 0;
                for (int j = 0; j < nVariaveisFolga; j++)
                {
                    if (varBase[j] == i)
                        achou = 1;
                }

                if (achou == 0)
                {
                    resultado[nVariaveisFolga+ktmp] = "X" + i.ToString() + " = " + "0";
                    ktmp++;
                }

            }
            /*
            for (int i = 0; i < nVariaveisFolga; i++)
            {
                listBox2.Items.Add(varBase[i].ToString());
            }
            */
            //exibir resultados na label
            //lblResultado.Text ="["+resultado[0] +"] " + "[" + resultado[1] + "] " + "[" + resultado[2] + "] " + "[" + resultado[3] + "] ";
            String resTemp = "";

            for (int i = 0; i < nVariaveisFolga+nVariaveis; i++)
            {
                resTemp += "[" + resultado[i] + "] ";
            }
            lblResultado.Text = resTemp + "[Z=" + matrizSimplex[0,nColunas-1] + "] ";
        }
    }
}
