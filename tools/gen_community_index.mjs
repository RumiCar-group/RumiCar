/**
 * コミュニティ投稿ディレクトリの index.json (マニフェスト) を生成する。
 * 置き場所は上流リポジトリ RumiCar-group/RumiCar の各ディレクトリ直下。
 *
 * なぜ要るか: シミュレータは投稿一覧を GitHub API で取っていたが、未認証 API は
 * IP あたり 60 回/時。教室のように 1 回線を共有すると数人で使い切ってしまう。
 * raw.githubusercontent.com にはこの制限が無いので、一覧を静的ファイルとして
 * 置けば API を使わずに済む (シミュレータ v7.4.0 が index.json を優先して読む)。
 *
 * 出力形式 (シミュレータ loader.js の listDirCached が読む):
 *   { "generated": "<ISO8601>", "entries": ["<ファイル名>", ...] }
 *   ※ 配列だけの ["a.json", ...] も受け付ける。index.json 自身は必ず除外する。
 *   ※ generated は情報用でシミュレータは読まない。冪等性は entries の一致で判定するので、
 *     手編集で改行コードやインデントだけが違うファイルは書き直さない (中身は同値)。
 *
 * 実行: node gen_community_index.mjs <リポジトリのルート>
 */
import { readdirSync, writeFileSync, readFileSync, existsSync, statSync } from 'node:fs';
import { join } from 'node:path';

// 対象は「ファイルが並ぶディレクトリ」だけ。races/ を足してはいけない —
// シミュレータの公式レース一覧はディレクトリ (eventId) を数えるもので、この生成器は
// ファイルしか列挙できない。races/index.json を置くと entries:[] = 「大会 0 件・正常」
// と読めてしまうため、シミュレータ側 (loader.js listDirCached) は dirs 一覧では
// マニフェストを読まない実装にしてある。両側で二重に塞いでいる。
// 大会エントリー (races/<id>/entries/) はファイルが並ぶので対象にしてよい。
const DIRS = ['courses/community', 'programs/community', 'cars/community'];

const root = process.argv[2];
if (!root) { console.error('使い方: node gen_community_index.mjs <リポジトリのルート>'); process.exit(2); }

let changed = 0;
let found = 0;
for (const d of DIRS) {
  const abs = join(root, d);
  if (!existsSync(abs) || !statSync(abs).isDirectory()) { console.log(`skip (無い): ${d}`); continue; }
  found++;
  // isFile() は lstat 相当なのでシンボリックリンクは false = 一覧に載せない。
  // raw から素直に取れないものを載せない方が安全なので、これは意図した挙動。
  // index.json の除外は大小無視 (index.JSON という名前で門をすり抜けさせない)。
  const entries = readdirSync(abs, { withFileTypes: true })
    .filter((e) => e.isFile() && e.name.toLowerCase() !== 'index.json' && !e.name.startsWith('.'))
    .map((e) => e.name)
    .sort();                                   // 並びを固定 = 差分が出るのは中身が変わったときだけ
  const out = join(abs, 'index.json');
  const body = JSON.stringify({ generated: new Date().toISOString(), entries }, null, 2) + '\n';
  // entries が同じなら書かない (generated だけが動く無意味なコミットを避ける)
  let prev = null;
  try { prev = JSON.parse(readFileSync(out, 'utf8')); } catch (e) { prev = null; }
  if (prev && Array.isArray(prev.entries) && JSON.stringify(prev.entries) === JSON.stringify(entries)) {
    console.log(`変更なし: ${d}/index.json (${entries.length} 件)`);
    continue;
  }
  writeFileSync(out, body);
  changed++;
  console.log(`書き出し: ${d}/index.json (${entries.length} 件)`);
}
// 1 つも見つからないのは「投稿が無い」ではなく「実行場所が違う」。
// 黙って成功すると、CI が緑のまま一覧が更新されない状態が続くので落とす。
if (found === 0) {
  console.error(`対象ディレクトリが 1 つも見つからない (root=${root})。リポジトリのルートを渡すこと`);
  process.exit(1);
}
console.log(changed ? `更新 ${changed} 件` : '更新なし');
