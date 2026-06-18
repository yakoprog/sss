# mini_shell-on-progress-


*** Workflow - Basic:

1. **Kod Yaz:** VS Code veya Vim ile kodunu yaz, kaydet.
2. **Terminale Geç:** `git status` yaz (hangi dosyalar değişmiş gör).
3. **Sahneye Al:** `git add .`
4. **Paketle:** `git commit -m "Mesajın"`
5. **Gönder:** `git push origin develop` (veya çalıştığın branch).

*** Working at Branch - Basic:
# 1. Başla
git checkout develop
git pull origin develop

# 2. Dal Aç
git checkout -b feature/GOREV-ADI

# ... Kod Yaz ...

# 3. Kaydet
git add .
git commit -m "feat: yaptığın işin özeti"

# 4. Gönder
git push origin feature/GOREV-ADI



*** MiniShell - Açıklama

Minishell, dışarıdan bakınca karmaşık görünse de aslında **"Sonsuz Bir Döngü"** içinde çalışan bir fabrikadır.

Mantığı kafanda oturtman için sana bunu bir **Restoran Mutfağı** metaforuyla ve gerçek kod akışıyla anlatayım.

Genel akış şöyledir: **OKU (Read) -> PARÇALA (Parse) -> ÇALIŞTIR (Execute) -> TEMİZLE (Free)**

---

### 1. The Loop (Garson - Siparişi Alma)

Programın main fonksiyonu bir `while(1)` döngüsüdür. Kullanıcı "exit" diyene kadar program kapanmaz.

* **Prompt:** Ekrana `minishell$ ` yazısını basarsın.
* **Readline:** Kullanıcının yazdığı satırı okursun.
* **History:** Yazılanı geçmişe (`add_history`) eklersin (yukarı ok tuşuyla gelsin diye).
* **Sinyaller:** Kullanıcı burada `Ctrl+C` basarsa yeni satıra geçmeli, programı kapatmamalı.

---

### 2. The Lexer (Hazırlıkçı - Malzemeyi Doğrama)

Kullanıcıdan gelen ham stringi (yazıyı), bilgisayarın anlayacağı **"Token"**lara (kelimelere) ayırma işlemidir.

**Örnek Girdi:**
`ls -la | grep "dosya 42"`

**Lexer'ın Yapacağı İş:**
Bu uzun yazıyı alır ve bağlı liste (linked list) haline getirir:

1. `WORD`: "ls"
2. `WORD`: "-la"
3. `PIPE`: "|"
4. `WORD`: "grep"
5. `WORD`: "dosya 42" *(Dikkat: Tırnak içinde olduğu için tek parça kaldı!)*

> **En Zor Kısım:** Tırnak işaretleri (`'` ve `"`). Tırnak içindeki boşluklar ayrılmaz, tırnak içindeki özel karakterler (pipe gibi) işlevsizleşir.

---

### 3. The Expander (Çevirmen)

Tokenların içindeki dolar işaretli (`$`) değişkenleri gerçek değerleriyle değiştirirsin.

* Girdi: `echo $USER`
* Çıktı: `echo ahmet`

---

### 4. The Parser (Şef - Tabaklama)

Dağınık duran tokenları, çalıştırılabilir **Komut Paketlerine** (struct) dönüştürürsün. Pipe (`|`) gördüğü yerden listeyi böler.

**Elinizdeki Veri Yapısı Şuna Dönüşmeli:**

* **Komut 1:**
* Program: `ls`
* Argümanlar: `["ls", "-la", NULL]`
* Girdi: Standart Input (Klavye)
* Çıktı: **PIPE** (Ekrana değil, boruya yazacak)


* **Komut 2:**
* Program: `grep`
* Argümanlar: `["grep", "dosya 42", NULL]`
* Girdi: **PIPE** (Boru'dan okuyacak)
* Çıktı: Standart Output (Ekran)



---

### 5. The Executor (Aşçı - Pişirme)

Burası işin "System Call" kısmıdır. Parser'dan gelen paketleri sırayla çalıştırırsın. Burada iki yol ayrımı var:

#### Yol A: Built-in Komutlar (Ev Yapımı Yemekler)

Eğer komut `cd`, `export`, `unset`, `exit`, `echo`, `pwd`, `env` ise;

* Bunları **fork yapmadan**, doğrudan kendi yazdığın C fonksiyonlarıyla çalıştırırsın.
* Örneğin `cd` için `chdir()` fonksiyonunu kullanırsın.
* **Neden?** Çünkü `cd` işlemini child process'te yaparsan, ana programın (shell'in) bulunduğu klasör değişmez.

#### Yol B: Binary Komutlar (`/bin/ls`, `/bin/cat`...)

Eğer komut sistem komutuysa:

1. **Fork():** Kendini kopyala (Child Process).
2. **Redirections:** Eğer `<` veya `>` varsa, `dup2()` ile dosya yollarını ayarla.
3. **Pipe:** Eğer boru varsa, input/output'u boruya bağla.
4. **Execve():** `ls` programını çalıştır. Bu noktada child process `ls` programına dönüşür.
5. **Waitpid():** Ana process (Parent), çocuğun işini bitirmesini bekler.

---

### Özet: Nereden Başlamalı?

Bu yapının tamamını aynı anda kuramazsın. Adım adım gitmen gereken sıra şudur:

1. **Altyapı:** Main döngüsü, `readline` ile yazı okuma.
2. **Veri Yapısı:** Tokenlar ve Komutlar için `struct` tasarlama. (Burası projenin omurgasıdır).
3. **Basit Lexer:** Sadece boşluklara göre ayıran bir yapı.
4. **Basit Executor:** Sadece tek bir komutu (`/bin/ls`) çalıştıran yapı.

**Sana bir "Struct" (Veri Yapısı) taslağı hazırlayayım mı?** Çünkü Lexer ve Parser yazmaya başlamadan önce veriyi nasıl tutacağını bilmen şart.
