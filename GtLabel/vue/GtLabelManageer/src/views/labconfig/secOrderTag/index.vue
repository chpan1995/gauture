<template>
  <div>
    <div
      style="display: flex; align-items: center; gap: 10px; margin-bottom: 24px"
    >
      <p>种类：</p>
      <el-select v-model="selectValue" size="large" style="width: 240px">
        <el-option
          v-for="item in options"
          :key="item.value"
          :label="item.label"
          :value="item.value"
        />
      </el-select>
      <el-button type="primary" @click="dialogVisible = true">
        新增配置
      </el-button>
    </div>
    <el-card>
      <template v-for="(item, index) in testone" :key="index">
        <div v-if="item.items">
          <div
            style="
              display: flex;
              align-items: center;
              gap: 15px;
              margin-bottom: 24px;
            "
          >
            <span class="first_type_span"> 一级分类 </span>
            <span class="first_value_span"> {{ item.value }} </span>
          </div>
          <div
            class="first_tags"
            v-for="(item, index) in item.items"
            :key="index"
          >
            <span class="first_tags_span"> 一级标签 </span>
            <span class="first_tags_value"> {{ item.value }} </span>
            <div
              class="second_tag"
              v-for="(item, index) in item.items"
              :key="index"
            >
              <span class="second_tags_span"> 二级标签 </span>
              <span class="second_tags_value"> {{ item.value }} </span>
            </div>
          </div>
        </div>
        <!-- 分割线，避免最后一个元素也有分割线 -->
        <hr v-if="index !== testone.length - 1" class="divider" />
      </template>
    </el-card>
    <el-dialog
      v-model="dialogVisible"
      title="新增配置"
      width="904"
      style="height: 700px"
      @close="resetData"
    >
      <div class="dia" style="height: 630px; overflow: auto">
        <input
          type="file"
          ref="fileInputRef"
          @change="handleFileUpload"
          accept=".json"
        />
        <el-input
          v-model="fileValue"
          style="width: 100%; margin-top: 10px; margin-bottom: 10px"
          :rows="2"
          type="textarea"
          placeholder="Please input"
          resize="none"
        >
        </el-input>
        <!-- 这里使用 Flexbox 让按钮靠右 -->
        <div
          style="
            display: flex;
            justify-content: flex-end;
            margin-top: 10px;
            margin-right: 10px;
            margin-bottom: 30px;
          "
        >
          <el-button type="primary" @click="parseJson">解析</el-button>
        </div>
        <template v-for="(item, index) in tags" :key="index">
          <div v-if="item.items">
            <div
              style="
                display: flex;
                align-items: center;
                gap: 15px;
                margin-bottom: 24px;
              "
            >
              <span class="first_type_span"> 一级分类 </span>
              <span class="first_value_span"> {{ item.value }} </span>
            </div>
            <div
              class="first_tags"
              v-for="(item, index) in item.items"
              :key="index"
            >
              <span class="first_tags_span"> 一级标签 </span>
              <span class="first_tags_value"> {{ item.value }} </span>
              <div
                class="second_tag"
                v-for="(item, index) in item.items"
                :key="index"
              >
                <span class="second_tags_span"> 二级标签 </span>
                <span class="second_tags_value"> {{ item.value }} </span>
              </div>
            </div>
          </div>
          <!-- 分割线，避免最后一个元素也有分割线 -->
          <hr v-if="index !== tags.length - 1" class="divider" />
          <div
            style="
              display: flex;
              justify-content: flex-end;
              margin-top: 10px;
              margin-right: 10px;
            "
          >
            <el-button
              v-if="index === tags.length - 1 && item.value"
              type="primary"
            >
              保存
            </el-button>
          </div>
        </template>
      </div>
    </el-dialog>
  </div>
</template>

<script setup lang="ts">
import { ref } from "vue";

const selectValue = ref("wheat");
const dialogVisible = ref(false);
const fileValue = ref<string>("");
import { ElMessageBox } from "element-plus";
const tags = ref<any[]>();

const fileInputRef = ref();

const options = [
  {
    value: "wheat",
    label: "小麦",
  },
  {
    value: "corn",
    label: "玉米",
  },
];

const testone = [
  {
    value: "颗粒类型",
    items: [
      {
        value: "病斑",
        items: [
          {
            value: "赤霉",
            items: [],
          },
          {
            value: "黑胚",
            items: [],
          },
          {
            value: "呆白皱缩",
            items: [],
          },
          {
            value: "开裂",
            items: [],
          },
          {
            value: "黑色子囊壳",
            items: [],
          },
        ],
      },
      {
        value: "生芽",
        items: [
          {
            value: "裂口",
            items: [],
          },
          {
            value: "短芽",
            items: [],
          },
          {
            value: "长芽",
            items: [],
          },
          {
            value: "胡须",
            items: [],
          },
          {
            value: "无麦胚",
            items: [],
          },
        ],
      },
      {
        value: "生霉",
        items: [
          {
            value: "类似灰尘",
            items: [],
          },
        ],
      },
      {
        value: "虫蚀",
        items: [
          {
            value: "啃咬",
            items: [],
          },
          {
            value: "孔洞",
            items: [],
          },
        ],
      },
      {
        value: "破损",
        items: [
          {
            value: "二分之一",
            items: [],
          },
          {
            value: "四分之三",
            items: [],
          },
          {
            value: "开裂",
            items: [],
          },
        ],
      },
      {
        value: "热损伤",
        items: [
          {
            value: "自然",
            items: [],
          },
          {
            value: "烘干",
            items: [],
          },
        ],
      },
      {
        value: "正常",
        items: [
          {
            value: "破皮",
            items: [],
          },
          {
            value: "裂纹",
            items: [],
          },
        ],
      },
      {
        value: "杂质",
        items: [
          {
            value: "有机",
            items: [],
          },
          {
            value: "无机",
            items: [],
          },
          {
            value: "无使用价值",
            items: [],
          },
          {
            value: "虫",
            items: [],
          },
        ],
      },
      {
        value: "未熟粒",
        items: [
          {
            value: "破皮",
            items: [],
          },
          {
            value: "裂纹",
            items: [],
          },
        ],
      },
      {
        value: "带壳粒",
        items: [],
      },
      {
        value: "未知",
        items: [],
      },
    ],
  },
];

const handleFileUpload = (event) => {
  const file = event.target.files[0]; // 获取文件
  if (!file) return;

  const reader = new FileReader();

  reader.onload = (e) => {
    fileValue.value = e.target.result as string; // 读取到的文件内容
  };

  reader.readAsText(file); // 以文本格式读取
};

const parseJson = () => {
  try {
    let obj = JSON.parse(fileValue.value);
    if (Array.isArray(obj)) {
      tags.value = obj;
    } else {
      ElMessageBox.alert("内容输入有误", { confirmButtonText: "确定" });
    }
  } catch (error) {
    ElMessageBox.alert("内容输入有误", { confirmButtonText: "确定" });
  }
};

const resetData = () => {
  tags.value = [];
  fileValue.value = "";
  fileInputRef.value.value = "";
};
</script>

<style lang="scss">
.first_type_span {
  font-size: 14px;
  color: #333333;
}
.first_value_span {
  display: flex;
  justify-content: center; /* 水平居中 */
  align-items: center; /* 垂直居中 */
  background: #f1f3f4;
  border-radius: 4px;
  border: 1px solid #cbcbcb;
  width: 224px;
  height: 40px;
  color: #333333;
}
.first_tags {
  display: flex;
  align-items: center; /* 垂直居中 */
  gap: 15px; // 控制间距
  margin-bottom: 24px;
  flex-wrap: wrap;
  .first_tags_span {
    font-size: 14px;
    color: #666666;
    // margin-right: 10px;
  }
  .first_tags_value {
    // display: flex;
    // justify-content: center; /* 水平居中 */
    // align-items: center; /* 垂直居中 */
    // display: inline-block;
    background: #f1f3f4;
    border-radius: 4px;
    border: 1px solid #cbcbcb;
    width: 224px;
    height: 40px;
    color: #333333;
    line-height: 40px;
    text-align: center;
  }
  .second_tag {
    display: flex;
    align-items: center; /* 垂直居中 */
    gap: 15px; // 控制间距
    // margin-left: 340px;
    // margin-top: -34px;
    // margin-bottom: 64px;
    .second_tags_span {
      font-size: 14px;
      color: #666666;
    }
    .second_tags_value {
      display: flex;
      justify-content: center; /* 水平居中 */
      align-items: center; /* 垂直居中 */
      background: #f1f3f4;
      border-radius: 4px;
      border: 1px solid #cbcbcb;
      width: 100px;
      height: 40px;
      color: #333333;
    }
  }
  .divider {
    border: none;
    height: 1px;
    background: #e0e0e0;
    margin: 16px 0;
  }
}
.el-textarea__inner {
  height: 360px !important;
}
</style>