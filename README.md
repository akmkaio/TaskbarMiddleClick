# TaskbarMiddleClick — compilação automática no GitHub

Este projeto tenta mostrar a área de trabalho quando você clica com o botão do meio em uma área vazia da barra de tarefas do Windows.

## Como obter o EXE
1. No GitHub, crie um repositório novo chamado `TaskbarMiddleClick`.
2. Deixe-o como **Private** ou **Public** (qualquer um funciona) e crie o repositório.
3. Na página do repositório, clique em **Add file → Upload files**.
4. Extraia este ZIP no computador e envie todos os arquivos e pastas que estão dentro dele. Importante: inclua a pasta oculta `.github` e o arquivo `.github/workflows/build.yml`.
5. Clique em **Commit changes**.
6. Abra a aba **Actions** do repositório. O workflow **Build Windows EXE** deve começar automaticamente depois do commit.
7. Clique na execução mais recente concluída (verde).
8. Na seção **Artifacts**, clique em `TaskbarMiddleClick-Windows-x64` para baixar o ZIP com o `TaskbarMiddleClick.exe`.
9. Extraia o ZIP baixado e execute o `.exe`.

## Se o workflow não iniciar
- Vá para **Actions** e habilite os workflows, se o GitHub pedir.
- Abra **Build Windows EXE** e clique em **Run workflow** → **Run workflow**.

## Observações
- O executável é compilado para Windows x64.
- O artefato do Actions fica disponível por 30 dias; baixe e guarde uma cópia.
- Na primeira execução, o Windows pode mostrar um aviso do SmartScreen para um executável pessoal sem assinatura. Só execute se você confiar no código e no repositório.
- Para encerrar o utilitário, finalize `TaskbarMiddleClick.exe` no Gerenciador de Tarefas.
- Para iniciar com o Windows, pressione Win+R, digite `shell:startup` e coloque nessa pasta um atalho para o EXE.
- A detecção de espaço vazio pode variar conforme a versão e configuração da barra de tarefas. Teste antes de configurar a inicialização automática.
